#!/bin/sh
if [ "$#" != "2" ]; then
	echo "usage: ./makebin.sh [sce file] [bin file]"
	exit 1
fi
python parse/SCEparser.py sce/$1 bin/$2