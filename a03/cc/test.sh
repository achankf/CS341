#!/bin/bash

while [ $? -eq 0 ] ; do
	./randpair 10 1 1 | ./a03q8.exe
done
