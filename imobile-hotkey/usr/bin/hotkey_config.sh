#!/bin/bash

# init buttons[] and commands[] from resource file, where will be in the work dir or home dir
buttons=("B1" "B2" "B3" "B4" "B5")
commands=("" "" "" "" "")

function save_rc()
{
  rm -f $RC; touch $RC
  for (( i=0; i<${#buttons[@]}; i++ )); do
    echo "${buttons[$i]}:${commands[$i]}" >> $RC
  done
}

#function dump_rc()
#{
#  echo "========================================================"
#  for (( i=0; i<${#buttons[@]}; i++ )); do
#    echo "${buttons[$i]}:${commands[$i]}"
#  done
#}

# prepare $RC file
RC=
[ -e "buttons_client.rc" ] && RC=buttons_client.rc
[ "x$RC" = "x" ] && [ -e "$HOME/.buttons_client.rc" ] && RC=$HOME/.buttons_client.rc
[ "x$RC" = "x" ] && [ -e "/usr/share/doc/imobile-hotkey/buttons_client.rc" ] && RC=$HOME/.buttons_client.rc && cp -a /usr/share/doc/imobile-hotkey/buttons_client.rc $RC
if [ "x$RC" = "x" ]; then
  RC=$HOME/.buttons_client.rc
  buttons[0]="B1"; buttons[1]="B2"; buttons[2]="B3";buttons[3]="B4";buttons[4]="B5";
  commands[0]="/usr/bin/firefox"
  commands[1]="/usr/bin/xeyes"
  commands[2]="/usr/bin/xterm"
  commands[3]="/usr/bin/oclock || /usr/bin/xclock"
  commands[4]="/usr/bin/gnome-calculator"
  save_rc
fi

while read line; do
  button=${line%:*}
  command=${line#*:}
  case "$button" in
    "B1") commands[0]="$command" ;;
    "B2") commands[1]="$command" ;;
    "B3") commands[2]="$command" ;;
    "B4") commands[3]="$command" ;;
    "B5") commands[4]="$command" ;;
  esac
done < $RC

# prepare user startup script ...
if [ ! -e $HOME/.config/upstart/buttons_client.conf ]; then
  cat <<CLIENT > $HOME/.config/upstart/buttons_client.conf
description "iMobile Hotkey Client"
start on desktop-start
stop on desktop-end

script
       /usr/bin/buttons_client
end script
CLIENT
fi
# and then start it
pidof buttons_client || start buttons_client

# prepare desktop icon
if [ ! -e $HOME/Desktop/Hotkey-Config.desktop ]; then
  cp -a /usr/share/applications/Hotkey-Config.desktop $HOME/.local/share/applications
fi

# make menu GUI on status bar
# icon 路徑在 /usr/share/icons/gnome/scalable/devices
function get_app()
{
  BTN=$1
  APP=$2
  [ "x$BTN" = "x" ] && BTN="Button"
  zenity --entry --title $BTN --text="Input application full path:" --entry-text="$APP" 2>/dev/null
}

function stop_client()
{
  stop buttons_client
  pidof buttons_client && killall buttons_client
}

function start_client()
{
  start buttons_client
  pidof buttons_client || /usr/bin/buttons_client&
  
}

function restart_client()
{
  stop_client
  start_client
}

function run_config()
{
  (cat <<MENU
Button 1
Button 2
Button 3
Button 4
Button 5

buttons client:restart
buttons client:stop
buttons client:start
MENU
  ) | /usr/bin/cappind.py -p -i input-dialpad-symbolic -l Hotkey | while read s; do
    case "$s" in
      "Button 1") APP=`get_app Btn1 "${commands[0]}"`; [ ! "x$APP" = "x" ] && commands[0]="$APP";;
      "Button 2") APP=`get_app Btn2 "${commands[1]}"`; [ ! "x$APP" = "x" ] && commands[1]="$APP";;
      "Button 3") APP=`get_app Btn3 "${commands[2]}"`; [ ! "x$APP" = "x" ] && commands[2]="$APP";;
      "Button 4") APP=`get_app Btn4 "${commands[3]}"`; [ ! "x$APP" = "x" ] && commands[3]="$APP";;
      "Button 5") APP=`get_app Btn5 "${commands[4]}"`; [ ! "x$APP" = "x" ] && commands[4]="$APP";;
      "buttons client:stop") stop_client;;
      "buttons client:start") start_client;;
      "buttons client:restart") restart_client;;
    esac
  done
}

run_config&
