/* server.c */
 
#include <dbus/dbus.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <strings.h>
 
typedef struct {
  char button[10];
  char command[255];
} BUTTON;

BUTTON buttons[5];

static DBusHandlerResult
filter_func(DBusConnection *connection, DBusMessage *message, void *usr_data)
{
    dbus_bool_t handled = false;
    char *word = NULL;
    DBusError dberr;
    int count=0;
 
    if (dbus_message_is_signal(message, "imobile.buttonsd.Type", "Pressed")) {
        dbus_error_init(&dberr);
        dbus_message_get_args(message, &dberr, DBUS_TYPE_STRING, &word, DBUS_TYPE_INVALID);
        if (dbus_error_is_set(&dberr)) {
            dbus_error_free(&dberr);
        } else {
            for (count=0; count<5; count++) {
              if ((strstr(word, "button:") == word) && (strstr(word, buttons[count].button) == (word+7))) {
                system(buttons[count].command);
                break;
              }
            }
            handled = true;
        }
    }
    return (handled ? DBUS_HANDLER_RESULT_HANDLED : DBUS_HANDLER_RESULT_NOT_YET_HANDLED);
}
 
int main(int argc, char *argv[])
{
    DBusError dberr;
    DBusConnection *dbconn;
    FILE *fp;
    char *line, *idx;
    size_t len=0;
    ssize_t read;
    int count=0;

    fp = fopen("buttons_client.rc", "r");
    if (NULL == fp) {
      chdir(getenv("HOME"));
      fp = fopen(".buttons_client.rc", "r");
      if (NULL == fp) return -1;
    }
    while ((read = getline(&line, &len, fp)) != -1) {
      if ((idx = strchr(line, '\r')) != NULL) *idx = '\0';
      if ((idx = strchr(line, '\n')) != NULL) *idx = '\0';
      if ((idx = strchr(line, ':')) != NULL) {
        strncpy (buttons[count].button, line, (idx-line));
        strcpy (buttons[count].command, idx+1);
      }
      count++;
    }
    fclose (fp);

    dbus_error_init(&dberr);
 
    dbconn = dbus_bus_get(DBUS_BUS_SESSION, &dberr);
    if (dbus_error_is_set(&dberr)) {
        dbus_error_free(&dberr);
        return -1;
    }
 
    if (!dbus_connection_add_filter(dbconn, filter_func, NULL, NULL)) {
        return -1;
    }
 
    dbus_bus_add_match(dbconn, "type='signal',interface='imobile.buttonsd.Type'", &dberr);
    if (dbus_error_is_set(&dberr)) {
        dbus_error_free(&dberr);
        return -1;
    }
 
    while(dbus_connection_read_write_dispatch(dbconn, -1)) {
        /* loop */
    }
    return 0;
}
