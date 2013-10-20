IN= $(shell find -name "*.cc")
OUT= $(IN:.cc=.exe)

all: $(OUT)

%.exe : %.cc
	g++ -DMY_DEBUG -ggdb -O0 $< -o $@

.phony: clean
clean:
	rm -f $(OUT)
