#include <dbus/dbus.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
 
int db_send(char* word)
{
    char msg[255];

    sprintf(msg, "/usr/bin/dbus-send --session --type=signal / imobile.buttond.Type.Pressed string:\"%s\"", word);
    system(msg);
    return 0;
}
 
int main(int argc, char *argv[])
{
    if (argc > 1)
      db_send(argv[1]);
    else
      db_send("Hello world!");
 
    return 0;
}
