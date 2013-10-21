IN= $(shell find -name "*.cpp")
OUT= $(IN:.cpp=.exe) randpair.exe

all: $(OUT)

%.exe : %.cpp
	g++ -DALFRED_CHAN_DEBUG -ggdb -O0 $< -o $@

randpair.exe : randpair.c
	gcc -O3 $< -o $@

.phony: clean
clean:
	rm -f $(OUT)
