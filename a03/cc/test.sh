#!/bin/bash

make
while [ $? -eq 0 ] ; do
	./randpair.exe 10 1 1 > test.out
	./a03q8.exe < test.out > test.log
	if [ $? -ne 0 ]; then
		cat test.out test.log
		exit
	fi
done
