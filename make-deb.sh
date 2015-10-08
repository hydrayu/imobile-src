# 本檔案專供 imobile-hotkey 包裝成 ubuntu deb 套件使用
PKGDIR=$1
[ "x$PKGDIR" = "x" ] && PKGDIR=imobile-hotkey
cd $PKGDIR
find . -type f ! -path './DEBIAN/*' -printf '%P\0' | sort -z | xargs -r0 md5sum > DEBIAN/md5sums
cd ..
fakeroot dpkg -b $PKGDIR
mv $PKGDIR.deb ~/tests/src/kernel
