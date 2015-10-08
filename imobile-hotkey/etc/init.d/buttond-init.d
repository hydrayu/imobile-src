#!/bin/sh

set -e

NAME=buttond
DAEMON=/usr/bin/$NAME
PIDDIR=/var/run/$NAME
PIDFILE=$PIDDIR/pid
DESC="imobile hotkey buttond, refer with buttons_client, buttond.conf, buttons_client.rc"

test -x $DAEMON || exit 0

. /lib/lsb/init-functions

# Source defaults file; edit that file to configure this script.
PARAMS=""
if [ ! -e /etc/buttond.conf ]; then
  cat << CONF > /etc/buttond.conf
A07 80 B1
A07 40 B2
A07 20 B3
A07 10 B4
A07 08 B5
CONF
fi

start_it_up()
{
  if [ ! -d $PIDDIR ]; then
    mkdir -p $PIDDIR
  fi

  if [ -e $PIDFILE ]; then
    if $0 status > /dev/null ; then
      log_success_msg "$DESC already started; not starting."
      return
    else
      log_success_msg "Removing stale PID file $PIDFILE."
      rm -f $PIDFILE
    fi
  fi

  log_daemon_msg "Starting $DESC" "$NAME"
  start-stop-daemon --start --quiet --pidfile $PIDFILE \
    --exec $DAEMON -- --system $PARAMS
  log_end_msg $?
}

shut_it_down()
{
  log_daemon_msg "Stopping $DESC" "$NAME"
  start-stop-daemon --stop --retry 5 --quiet --oknodo --pidfile $PIDFILE
  # We no longer include these arguments so that start-stop-daemon
  # can do its job even given that we may have been upgraded.
  # We rely on the pidfile being sanely managed
  # --exec $DAEMON -- --system $PARAMS
  log_end_msg $?
  rm -f $PIDFILE
  killall $NAME
}

case "$1" in
  start)
    start_it_up
  ;;
  stop)
    shut_it_down
  ;;
  reload|force-reload)
    shut_it_down
    start_it_up
  ;;
  restart)
    shut_it_down
    start_it_up
  ;;
  status)
    status_of_proc -p $PIDFILE $DAEMON $NAME && exit 0 || exit $?
    ;;
  *)
    echo "Usage: /etc/init.d/$NAME {start|stop|reload|restart|force-reload|status}" >&2
    exit 2
  ;;
esac

