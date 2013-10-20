#!/bin/bash

make

for file in `find -name "test*"`; do
	./a03q8.exe < $file > /dev/null
	[ $? -ne 0 ] && echo $file && exit
done
