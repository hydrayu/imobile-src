CONT=true
while $CONT; do
case "`(cat <<EOF
_Aa
b:1
b:2
b:
b:q

c
EOF
) | ./cappind.py`" in
    _Aa ) echo hello ;;
    b:1 ) echo big ;;
    b:2 ) echo small ;;
    b:q ) export CONT=false ;;
    c ) echo world ;;
esac
done
