#!/bin/bash

(echo "Home Folder
Documents
Downloads
Pictures
Music
Videos
"
cat $HOME/.gtk-bookmarks | cut -f1 -d' ' |
    while read s; do basename $s; done
) | ./cappind.py -p -i input-dialpad-symbolic -l Hotkey | while read s; do
case "$s" in
Home\ Folder ) nautilus & ;;
Documents ) nautilus ~/Documents/ & ;;
Downloads ) nautilus ~/Downloads/ & ;;
Pictures ) nautilus ~/Pictures/ & ;;
Music ) nautilus ~/Music/ & ;;
Videos ) nautilus ~/Videos/ & ;;
* )
    nautilus `cat $HOME/.gtk-bookmarks | cut -f1 -d' ' |
        while read s; do echo "\`basename $s\`/$s"; done | grep "^$s" |
        cut -d/ -f2-` &
    ;;
esac
done
# icon 路徑在 /usr/share/icons/gnome/scalable/devices
# system-devices-panel
