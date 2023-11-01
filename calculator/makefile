make: 	Calculator.h main.cpp
	g++ -o test Calculator.h main.cpp
	./test
	@echo "Input"
	@cat input.txt
	@echo ""
	@echo "Output"
	@cat output.txt

report: report.tex
	xelatex report
	xelatex report
	rm report.aux
	rm report.log
	rm report.out	
