#!/bin/bash

export DINGUXDEV=/opt/opendingux-toolchain
#export RUNTIME_PREFIX=${DINGUXDEV}/usr/lib
#export PREFIX=${DINGUXDEV}/usr/lib
#export DEVEL_PREFIX=${DINGUXDEV}/usr/lib
#export TOPDIR=${DINGUXDEV}/usr/lib

#export SHARED_LIB_LOADER_PREFIX=${DINGUXDEV}/usr/lib
#export RUNTIME_PREFIX=${DINGUXDEV}/usr/lib
#export DEVEL_PREFIX="/usr/"

make -f Makefile-opendingux

if [ $1 ]; then

ftp -n -i 10.1.0.2 << EOF
user opendingux
pwd
cd /mmcblk0p1/local/home/emulators/dingux-2600
mput dingux-2600
bye
EOF

fi