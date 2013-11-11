IN= $(shell find -name "*.cpp")
OUT= $(IN:.cpp=.exe)

all: $(OUT)

%.exe : %.cpp
	g++ -pedantic -Wall -Werror -DALFRED_CHAN_DEBUG -ggdb -O0 $< -o $@

.phony: clean
clean:
	rm -f $(OUT)
