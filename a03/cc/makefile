IN= $(shell find -name "*.cc")
OUT= $(IN:.cc=.exe) randpair

all: $(OUT)

%.exe : %.cc
	g++ -DALFRED_CHAN_DEBUG -ggdb -O3 $< -o $@

randpair : randpair.c
	gcc -O3 $< -o $@

.phony: clean
clean:
	rm -f $(OUT)
