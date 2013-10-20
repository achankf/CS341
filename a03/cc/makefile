IN= $(shell find -name "*.cc")
OUT= $(IN:.cc=.exe) randpair.exe

all: $(OUT)

%.exe : %.cc
	g++ -DALFRED_CHAN_DEBUG -ggdb -O0 $< -o $@

randpair.exe : randpair.c
	gcc -O3 $< -o $@

.phony: clean
clean:
	rm -f $(OUT)
