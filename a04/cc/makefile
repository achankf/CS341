IN= $(shell find -name "*.cpp")
OUT= $(IN:.cpp=.exe)
DEBUG= -DALFRED_CHAN_DEBUG

all: $(OUT)

%.exe : %.cpp
	g++ -pedantic -Wall -Werror $(DEBUG) -ggdb -O0 $< -o $@

.phony: clean
clean:
	rm -f $(OUT)
