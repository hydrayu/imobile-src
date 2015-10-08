#PORTS=`seq 1 1 1023`
#PORTS="1016 1017 1018 1019 1020 1021 1022 1023 2560 2561 2562 2563"
#PORTS="2560 2561 2562 2563 2564 2565 2566 2567"
PORTS="2561 2567"
function test_port {
  PORTNUM=$1
  OUTBYTE=$2
  ORG=`sudo ./buttond IB $PORTNUM | sed -e 's/.*: 0x//'`
  sudo ./buttond OB $PORTNUM $OUTBYTE
  NEW=`sudo ./buttond IB $1 | sed -e 's/.*: 0x//'`
  sudo ./buttond OB $PORTNUM $ORG
  PP=`sudo ./buttond IB $PORTNUM | sed -e 's/.*: 0x//'`
  if [ "$ORG" = "$PP" -a ! "$NEW" = "$ORG" ]; then
    [ "$NEW" = "$OUTBYTE" ] && RES="VV" || RES="OO"
  else
    RES="XX"
  fi
  stdbuf -o0 echo "$PORTNUM ($OUTBYTE) : [$RES] ORG=$ORG, NEW=$NEW, PP=$PP"
}

for p in $PORTS; do
  PORT=`printf "%x" $p`
  [ "x$PORT" = "x92" ] && continue
  for O in 0 1 2 4 8 16 32 64 128; do
    OUT=`printf "%x" $O`
    test_port $PORT $OUT
  done
done
