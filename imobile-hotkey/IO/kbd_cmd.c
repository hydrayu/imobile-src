/* kbd_cmd.c - aeb, 940505 */
/*
 * This program must have write permission to /dev/port - probably that
 * means that it has to be suid root.
 *
 * Without arguments: enable keyboard.
 *
 * With a single argument "sane": enable keyboard and set default scancodes.
 *
 * [These are useful commands, but when the keyboard is in some obscure
 * state they probably cannot be typed. Under X you can have them as a
 * menu item. Otherwise you might use "sleep 300; kbd_cmd sane" before
 * doing something dangerous, like playing with this program.]
 *
 * With a sequence of two-character hexadecimal values or symbolic commands:
 * send these values to the keyboard.
 * An argument W or w means wait.
 *
 * e.g., "kbd_cmd LED 7" will set the LEDs behind the kernel's back -
 * use the program "setleds" if you want the lights to mean anything.
 *
 * "kbd_cmd f0 1; showkey -s; kbd_cmd f0 2" will enable you to study the
 * scancodes produced in a different scancode mode.
 *
 * "kbd_cmd W f0 1 f0 0 W f0 2 & showkey -s" will enable you to find the
 * identifications of the various scancode sets, and similarly
 * "kbd_cmd W f2 & showkey -s" yields the keyboard identification.
 *
 * Playing with these things is dangerous! It is very easy to get into a
 * state in which the keyboard cannot be used anymore. Set up an escape
 * (as suggested above).
 * These commands work on my keyboard. For other keyboards the results
 * may be unpredictible.
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/file.h>
#include <string.h>

struct {
    unsigned char cmd;
    int argct, resct;
    char *name;
} commands[] = {
    0xed, 1, 0,   "LED",     /* arg 0-7: 1 ScrollLock, 2 NumLock, 4 CapsLock */
    0xee, 0, 1,   "echo",               /* result: ee */
    0xf0, 1, 1, "get_scancodes",          /* arg: 0, result: 43, 41 or 3f */
    0xf0, 1, 0,   "set_scancodes",          /* arg: 1-3 */
    0xf2, 0, 2,   "identify_keyboard",      /* result: ab 41 */
    0xf3, 1, 0,   "set_repeat_rate",
    0xf4, 0, 0,   "enable",
    0xf5, 0, 0,   "reset_and_disable",
    0xf6, 0, 0,   "reset_and_enable",
    0xfe, 0, 1,   "resend",
    0xff, 0, 1,   "reset_and_selftest"      /* result: aa (OK) / fc (error) */
};

int fd;
int args_expected;

send_cmd(unsigned char x) {
    char z;
    int i;

    do {
   lseek( fd, 0x64, 0 );
   read( fd, &z, 1 );
    } while ((z & 2) == 2 );  /* wait */

    lseek( fd, 0x60, 0 );
    write( fd, &x, 1 );

    if (args_expected)
      args_expected--;
    else {
   for(i=0; i<sizeof(commands)/sizeof(commands[0]); i++)
     if(x == commands[i].cmd) {
         args_expected = commands[i].argct;
         break;
     }
    }
}

int hexd(char c) {
    if ('0' <= c && c <= '9') return(c - '0');
    if ('a' <= c && c <= 'f') return(c - 'a' + 10);
    if ('A' <= c && c <= 'F') return(c - 'A' + 10);
    fprintf(stderr, "kbd_cmd: expected a hex digit, got _%c_ (0%o)\n", c, c);
    leave(1);
}

unsigned char tohex(char *s) {
    if(!s[0])
      return(0);
    else if(!s[1])
      return(hexd(s[0]));
    else
      return((hexd(s[0])<<4) + hexd(s[1]));
}

void main(int argc, char **argv) {
   int i, j;

   if ( (fd = open("/dev/port", O_RDWR)) < 0) {
      perror("Cannot open /dev/port");
      exit(1);
   }

   if (argc < 2) {
       send_cmd(0xf4);     /* enable */
   } else if (argc == 2 && !strcmp(argv[1], "sane")) {
       send_cmd(0);     /* Just in case the kbd was waiting */
            /* for the second byte of a command */
       send_cmd(0xf0);     /* Select scancode set */
       send_cmd(0x02);     /* set 2 */
       send_cmd(0xf4);     /* Enable keyboard */
   } else for (i=1; i<argc; i++) {
       if (!strcmp(argv[i], "W") || !strcmp(argv[i], "w")) {
      sleep(1);
       } else if (strlen(argv[i]) > 2) {
      for (j=0; j < sizeof(commands)/sizeof(commands[0]); j++)
        if(!strcmp(commands[j].name, argv[i])) {
       send_cmd(commands[j].cmd);
       goto fnd;
        }
      fprintf(stderr, "kbd_cmd: unrecognized command %s\n", argv[i]);
      leave(1);
    fnd:;
       } else
    send_cmd(tohex(argv[i]));
   }
   leave(0);
}

leave(int n) {
    /* prevent frozen keyboards, waiting for command arguments */
    while(args_expected)
      send_cmd(0);
    exit(n);
}


