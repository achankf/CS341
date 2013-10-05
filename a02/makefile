IN= $(shell find -name "*.tex")
INCLUDE = $(shell find -name "*.texin")
OUT= $(IN:.tex=.pdf)

all: $(OUT)

$(OUT):$(IN) $(INCLUDE)
	pdflatex $<
	pdflatex $<

.phony: clean
clean:
	rm -f *.log *.aux *.eps *eps-converted-to.pdf
cleanall:clean
	rm -f $(OUT)
