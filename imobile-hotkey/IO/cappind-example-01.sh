(cat <<EOF
_Aa
b:1
b:2
c
EOF
) | ./cappind.py --persist | while read s; do
case "$s" in
    _Aa ) echo hello ;;
    b:1 ) echo big ;;
    b:2 ) echo small ;;
    c ) echo world ;;
esac
done
