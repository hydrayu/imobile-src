#!/bin/bash

if [[ -z "$EXEC" ]]; then EXEC="./cappind.py"; fi

zenity --info --text="In the next screen, 
you must input the menu contents 
as lines of the form

<b><tt>menu:submenu: ... :entry</tt></b>

an empty entry meaning a separator"

MENU=`zenity --text-info --editable --title="Input menu contents"`
if [[ $? = "1" ]]; then exit; fi

ICON=`zenity --entry --title="Set an icon name" \
	--text="Choose an icon name from the theme" --entry-text="terminal"`
if [[ $? = "1" ]]; then exit; fi
if [[ -z "$ICON" ]]; then ICON="terminal"; fi

FILE=`mktemp`

(echo "#!/bin/bash

echo \"$MENU\" | $EXEC -p -i $ICON | while read s; do case \$s in"
for s in $MENU; do
	if [[ -z `echo "$s" | grep -e ":$"` ]]; then
		echo "$s" | sed "s/ /\\\\ /g" | sed "s/$/ \)/"
		echo "`zenity --entry --title="Choose command for this element" \
			--text="$s" || echo exit` & ;;"
		if [[ $? = "1" ]]; then exit; fi
	fi
done
echo "esac
done") > "$FILE"

DEST=`zenity --file-selection --title="Save script" --save \
	--confirm-overwrite --file-filter="Shell script | *.sh"`

mv "$FILE" "$DEST"
chmod +x "$DEST"

zenity --info --text="Indicator script created !"

