#!/bin/bash

# init buttons[] and commands[] from resource file, where will be in the work dir or home dir
buttons=()
commands=()
RC=
[ -e "buttons_client.rc" ] && RC=buttons_client.rc
[ "x$RC" = "x" ] && [ -e "$HOME/.buttons_client.rc" ] && RC=$HOME/.buttons_client.rc
while read line; do
  button=${line%:*}
  command=${line#*:}
  [ ! "x$command" = "x" ] && [ ! "x$button" = "x" ] && buttons+=("$button") && commands+=("$command")
done < $RC

# read dbus event and parse it, then call command
dbus-monitor --session "type='signal',path='/imobile/buttonsd/Object',interface='imobile.buttonsd.Type',member='Pressed'"  | sed -u -e '/signal.*/d' | sed -u -e '/":/d' | sed -u -e 's/.*\"button:\(.*\)\"/\1/' |
  while read line; do
    for (( i=0; i<${#buttons[@]}; i++)); do
      [ "x$line" = "x${buttons[$i]}" ] && ${commands[$i]}
    done
  done
