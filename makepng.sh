#!/bin/sh
if [ "$#" != "3" ]; then
	echo "usage: ./makepng.sh [bin file] [png file] [resolution]"
	echo "[resolution] options: low | high"
	exit 1
fi
make clean
make
./main -r $3 bin/$1 png/$2