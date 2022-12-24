#!/bin/sh
set -e
. ./build.sh

mkdir -p isodir
mkdir -p isodir/boot
mkdir -p isodir/boot/grub

cp sysroot/boot/myos.kernel isodir/boot/myos.kernel
cat > isodir/boot/grub/grub.cfg << EOF
menuentry "myos" {
  insmod all_video
  multiboot /boot/myos.kernel
}
EOF
grub2-mkrescue -o myos.iso isodir || grub-mkrescue -o myos.iso isodir
