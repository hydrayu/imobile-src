#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <sys/io.h> /* linux-specific */

#ifdef __GLIBC__
#  include <sys/perm.h>
#endif

#define PORT_FILE "/dev/port"

char *prgname;

#ifdef __i386__
static int read_and_print_one(unsigned int port,int size)
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
	printf("%04x: %08x\n", port, inl(port));
    else if (size == 2)
	printf("%04x: %04x\n", port, inw(port));
    else
	printf("%04x: %02x\n", port, inb(port));
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
        outl(val, port);
    else if (size == 2)
        outw(val&0xffff, port);
    else
        outb(val&0xff, port);
    return 0;
}

#else /* not i386 */

static int read_and_print_one(unsigned int port,int size)
{
    static int fd = -1;
    unsigned char b; unsigned short w; unsigned int l;

    if (fd < 0)
	fd = open(PORT_FILE, O_RDONLY);
    if (fd < 0) {
	fprintf(stderr, "%s: %s: %s\n", prgname, PORT_FILE, strerror(errno));
	return 1;
    }
    lseek(fd, port, SEEK_SET);
    
    if (size == 4) {
	read(fd, &l, 4);
	printf("%04x: 0x%08x\n", port, l);
    } else if (size == 2) {
	read(fd, &w, 2);
	printf("%04x: 0x%04x\n", port, w & 0xffff);
    } else {
	read(fd, &b, 1);
	printf("%04x: 0x%02x\n", port, b & 0xff);
    }
    return 0;
}

static int write_one(unsigned int port, unsigned int val, int size)
{
    static int fd = -1;
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

#endif /* i386 */

void print_usage()
{
  fprintf (stderr, "Usage: %s [IO][BWL] # # # #....\n\tWhere I(In), O(Out), B(Byte), W(Word), L(Long)\n\tFor Out mode, must give # pair\n", prgname);
}

int main(int argc, char **argv)
{
    unsigned int i, n, port, val, size, error = 0;
    char io, sc;
    
    prgname = argv[0];

    if (argc < 3 || strlen(argv[1]) < 2) {
      print_usage();
      return 1;
    }
    setuid(0); /* if we're setuid, force it on */
    io = argv[1][0];
    sc = argv[1][1];

    if (sc == 'b' || sc == 'B') size = 1;
    else if (sc == 'w' || sc == 'W') size = 2;
    else if (sc == 'l' || sc == 'L') size = 4;
    else {
      print_usage();
      return 1;
    }

    if (io == 'i' || io == 'I') {
      for (i = 2; i < argc; i++) {
        if ( sscanf(argv[i], "%x%n", &port, &n) < 1
	      || n != strlen(argv[i]) ) {
	    fprintf(stderr, "%s: argument \"%s\" is not a hex number\n",
		    argv[0], argv[i]);
	    error++; continue;
	}
	if (port & (size-1)) {
	    fprintf(stderr, "%s: argument \"%s\" is not properly aligned\n",
		    argv[0], argv[i]);
	    error++; continue;
	}
	error += read_and_print_one(port, size);
      }
    }
    else if (io == 'o' || io == 'O') {
      for (i=2;i<argc-1;i++) {
        if ( sscanf(argv[i], "%x%n", &port, &n) < 1
              || n != strlen(argv[i]) ) {
            fprintf(stderr, "%s: argument \"%s\" is not a hex number\n",
                    argv[0], argv[i]);
            error++; continue;
        }   
        if (port & (size-1)) {
            fprintf(stderr, "%s: argument \"%s\" is not properly aligned\n",
                    argv[0], argv[i]);
            error++; continue;
        }   
        if ( sscanf(argv[i+1], "%x%n", &val, &n) < 1
              || n != strlen(argv[i+1]) ) {
            fprintf(stderr, "%s: argument \"%s\" is not a hex number\n",
                    argv[0], argv[i+1]);
            error++; continue;
        }   
        if (size < 4 && val > (size == 1 ? 0xff : 0xffff)) {
            fprintf(stderr, "%s: argument \"%s\" out of range\n",
                    argv[0], argv[i+1]);
            error++; continue;
        }   
        error += write_one(port, val, size);
      }
    }
    exit (error ? 1 : 0);
}
