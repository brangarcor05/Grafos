GXX = g++
FLAGS = -Wall -std=c++11

PROGRAM = main

DEPENDENCYS = Grafo.cxx

$(PROGRAM):
	$(CXX) $(FLAGS) $@.cpp $(DEPENDENCYS) -o $@

clear:
	rm -rf $(PROGRAM)

update: clear $(PROGRAM)

run1:
	./$(PROGRAM) test_input1.txt

run2:
	./$(PROGRAM) test_input2.txt

run3:
	./$(PROGRAM) test_input3.txt