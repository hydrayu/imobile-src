/* 本檔只作為監聽 IO Port 使用，需要上層收聽它回報的 dbus 事件並處理 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/io.h> /* linux-specific */
#include <syslog.h>
#include <dbus/dbus.h>

using namespace std;

#define DAEMON 1

#ifdef __GLIBC__
#  include <sys/perm.h>
#endif

#define DAEMON_NAME "buttond"
#define PORT_FILE "/dev/port"

char *prgname;
static int fd = -1;
DBusConnection *dbconn;

// port config
typedef struct {
  unsigned int port;
  unsigned int mask;
  unsigned int pressed;
  char message[10];
} BUTTON;

#define MAX_BUTTONS 5

BUTTON buttons[MAX_BUTTONS] = {
  { 0xa07, 0x80, 0x0, "B1" },
  { 0xa07, 0x40, 0x0, "B2" },
  { 0xa07, 0x20, 0x0, "B3" },
  { 0xa07, 0x10, 0x0, "B4" },
  { 0xa07, 0x08, 0x0, "B5" },
};

#if defined(__i386__) || defined(__ia64__) || defined (__x86_64__)
int read_one(unsigned int port, int size, unsigned int* val)
{
    static int iopldone = 0;

    if (port > 1024) {
	if (!iopldone && iopl(3)) {
	    return -1;
	}
	iopldone++;
    } else if (ioperm(port,size,1)) {
	return -2;
    }

    if (size == 4)
	*val = inl(port) & 0xffffffff;
    else if (size == 2)
	*val = inw(port) & 0xffff;
    else
	*val = inb(port) & 0xff;
    return 0;
}

#if 0
static int read_and_print_one(unsigned int port,int size)
{
    unsigned int val;
    int res;

    res = read_one(port, size, &val);
    if (res == -1) {
      fprintf(stderr, "%s: iopl(): %s\n", prgname, strerror(errno));
      return 1;
    } else if (res == -2) {
	fprintf(stderr, "%s: ioperm(%x): %s\n", prgname,
		port, strerror(errno));
	return 1;
    }

    if (size == 4) printf("%04x: 0x%08x\n", port, val);
    else if (size == 2) printf("%04x: 0x%04x\n", port, val);
    else if (size == 1) printf("%04x: 0x%02x\n", port, val);
    return 0;
}

static int write_one(unsigned int port, unsigned int val, int size)
{
    static int iopldone = 0;

    if (port > 1024) {
        if (!iopldone && iopl(3)) {
            fprintf(stderr, "%s: iopl(): %s\n", prgname, strerror(errno));
            return 1;
        }
        iopldone++;
    } else if (ioperm(port,size,1)) {
        fprintf(stderr, "%s: ioperm(%x): %s\n", prgname,
                port, strerror(errno));
        return 1;
    }

    if (size == 4)
        outl(val&0xffffffff, port);
    else if (size == 2)
        outw(val&0xffff, port);
    else
        outb(val&0xff, port);
    return 0;
}
#endif
#else /* not i386 */

int read_one(unsigned int port, int size, unsigned* val)
{
    if (fd < 0)
	fd = open(PORT_FILE, O_RDONLY);
    if (fd < 0) {
	return -1;
    }
    lseek(fd, port, SEEK_SET);
    
    if (size == 4) {
	read(fd, &val, 4);
    } else if (size == 2) {
	read(fd, &val, 2);
    } else {
	read(fd, &val, 1);
    }
    return 0;
}
#if 0
static int read_and_print_one(unsigned int port,int size)
{
    unsigned int val;
    if (0 > read_one(port, size, &val)) {
      fprintf(stderr, "%s: %s: %s\n", prgname, PORT_FILE, strerror(errno));
      return 1;
    }
    if (size == 4) printf("%04x: 0x%08x\n", port, val & 0xffffffff);
    else if (size == 2) printf("%04x: 0x%04x\n", port, val & 0xffff);
    else if (size == 1) printf("%04x: 0x%02x\n", port, val & 0xff);
    return 0;
}

static int write_one(unsigned int port, unsigned int val, int size)
{
    unsigned char b; unsigned short w;

    if (fd < 0)
        fd = open(PORT_FILE, O_WRONLY);
    if (fd < 0) {
        fprintf(stderr, "%s: %s: %s\n", prgname, PORT_FILE, strerror(errno));
        return 1;
    }
    lseek(fd, port, SEEK_SET);

    if (size == 4) {
        write(fd, &val, 4);
    } else if (size == 2) {
        w = val;
        write(fd, &w, 2);
    } else {
        b = val;
        write(fd, &b, 1);
    }
    return 0;
}
#endif

#endif /* i386 */

void print_usage()
{
  fprintf (stderr, "Usage: %s {-d | CONFIG_FILE}\n"
           "-d : use default value\n"
           "If provide CONFIG_FILE, please make sure it is in correct format.\n"
           "In no argument, please check if /etc/buttond.conf exist or same as -d.\n"
           "\tFORMAT: PORT MASK MESSAGE\n"
           "\tONE BUTTON ONE LINE. where we support max 5 buttons now.\n"
           "\tNote1: Please make sure the config file is in correct format!\n"
           "\tNote2: Please provide PORT+MASK in HEX format.\n\n"
           "The sample /etc/buttond.conf as:\n"
           "A07 80 B1\n"
           "A07 40 B2\n"
           "A07 20 B3\n"
           "A07 10 B4\n"
           "A07 08 B5\n", prgname);
}

void parse_config(const char *file_name)
{
  int n;
  char message[10];
  FILE *fp = NULL;

  fp = fopen(file_name, "r");
  if (NULL == fp) {
    fprintf (stderr, "Please make sure the config file '%s' is correct!\n", file_name);
    return;
  }

  // reset config struct
  for (n=0; n<MAX_BUTTONS; n++) {
    buttons[n].port = 0;
    buttons[n].mask = 0;
    buttons[n].pressed = 0;
    memset (buttons[n].message, '\0', 10);
  }

  // read config file
  n = 0;
  do {
    if (fscanf (fp, "%x %x %s", &(buttons[n].port), &(buttons[n].mask), buttons[n].message) == 3) {
      n++;
    }
  } while (n<5 && !feof(fp));
  fclose (fp);
}

int db_send (DBusConnection *dbconn, char* msg)
{
  DBusMessage *dbmsg;
  
  dbmsg = dbus_message_new_signal("/imobile/buttond/Object", // object name of the signal
    "imobile.buttond.Type", // interface name of the signale
    "Pressed"); // name of the signal
  if (!dbmsg) return -1;
  if (!dbus_message_append_args(dbmsg, DBUS_TYPE_STRING, &msg, DBUS_TYPE_INVALID)) return -1;
  if (!dbus_connection_send(dbconn, dbmsg, NULL)) return -1;
  dbus_connection_flush(dbconn);
  dbus_message_unref(dbmsg);
  return 0;
}

void monitor_port() {
    int b;
    unsigned int val, pressed;
    char msg[20];

#ifdef DAEMON
    syslog (LOG_NOTICE, "Writing to my Syslog");
#endif
    for (b = 0; b < MAX_BUTTONS; b++) {
      if (read_one(buttons[b].port, 1, &val) == 0) {
        val &= 0xff;
        pressed = val & buttons[b].mask ? 0 : 1;
        if (pressed != buttons[b].pressed) {
          buttons[b].pressed = pressed;
          if (pressed) {
            sprintf(msg, "button:%s", buttons[b].message);
            db_send(dbconn, msg);
#ifndef DAEMON
            printf ("%s\n", msg);
#endif
            //printf ("%s port[%04X] value %02X, mask %02X == %02X %s\n", buttons[b].message,
            //  buttons[b].port, val, buttons[b].mask, val & buttons[b].mask, pressed?"pressed":"release");
          }
        }
      }
    }
}   

int main(int argc, char *argv[])
{
  setenv("DBUS_SESSION_BUS_ADDRESS", "unix:path=/run/dbus/system_bus_socket", 1);
  if (argc > 1) {
    if (strncmp (argv[1], "-h", 2) == 0) {
      print_usage();
      return 1;
    }
    if (strncmp (argv[1], "-d", 2) != 0 && access (argv[1], F_OK) == 0) {
      parse_config(argv[1]);
    }
  }
  else if (access ("/etc/buttond.conf", F_OK ) == 0) { // use default config file 
    parse_config("/etc/buttond.conf");
  }
  // dump config structure
#undef DUMP
#ifdef DUMP
  for (int n=0; n<MAX_BUTTONS; n++) {
    printf ("[0x%04X] [0x%02X] : '%s'\n", buttons[n].port, buttons[n].mask, buttons[n].message);
  }
#endif

    // create dbus
    DBusError dberr;

    dbus_error_init(&dberr);

    dbconn = dbus_bus_get(DBUS_BUS_SESSION, &dberr);
    if (dbus_error_is_set(&dberr)) {
	fprintf (stderr, "dbus_error_is_set() failed: %s\n", dberr.message);
        return -1;
    }

#ifdef DAEMON
    //Set our Logging Mask and open the Log
    setlogmask(LOG_UPTO(LOG_NOTICE));
    openlog(DAEMON_NAME, LOG_CONS | LOG_NDELAY | LOG_PERROR | LOG_PID, LOG_USER);

    syslog(LOG_NOTICE, "Entering Daemon");

    pid_t pid, sid;

    //Fork the Parent Process
    pid = fork();

    if (pid < 0) { exit(EXIT_FAILURE); }

    //We got a good pid, Close the Parent Process
    if (pid > 0) { exit(EXIT_SUCCESS); }

    //Change File Mask
    umask(0);

    //Create a new Signature Id for our child
    sid = setsid();
    if (sid < 0) { exit(EXIT_FAILURE); }
    //Change Directory
    //If we cant find the directory we exit with failure.
    if ((chdir("/")) < 0) { exit(EXIT_FAILURE); }

    //Close Standard File Descriptors
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

#endif
    //----------------
    //Main Process
    //----------------
    time_t sleep_milisec = 100;
    while(true) {

#ifdef DAEMON
        syslog(LOG_NOTICE, "call monitor_port()");
#endif
        monitor_port();    //Run our Process
#ifdef DAEMON
        syslog(LOG_NOTICE, "usleep(60)");
#endif
        usleep(sleep_milisec * 1000);
    }

#ifdef DAEMON
    syslog(LOG_NOTICE, "closelog()");
#endif
    //Close the log
    closelog ();
}
