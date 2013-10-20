#!/bin/bash

echo $1 $2 $3
make

for file in `find -name "test*"`; do
	./a03q8.exe < $file > /dev/null
	[ $? -ne 0 ] && echo $file && exit
done

while [ $? -eq 0 ] ; do
	./randpair.exe $1 $2 $3 > test.out
	./a03q8.exe < test.out > test.log
	if [ $? -ne 0 ]; then
		cat test.out test.log
		exit
	fi
done
