all: main.cpp grammar.o hash.o set.o lltable.o lexer.o lexerOutput.o symbolTableOutput.o quadRuplesOutput.o machineCodeOutput.o parse_tree.o
	g++ -g main.cpp build/*

grammar.o: grammar.h grammar.cpp
	g++ -g -o build/grammar.o -c grammar.cpp

hash.o: hash.h hash.cpp
	g++ -g -o build/hash.o -c hash.cpp

set.o: set.h set.cpp grammar.h
	g++ -g -o build/set.o -c set.cpp

lltable.o: lltable.h lltable.cpp
	g++ -g -o build/lltable.o -c lltable.cpp

lexer.o: lexer.h lexer.cpp
	g++ -g -o build/lexer.o -c lexer.cpp

lexerOutput.o: lexerOutput.h lexerOutput.cpp lexer.h
	g++ -g -o build/lexerOutput.o -c lexerOutput.cpp

symbolTableOutput.o: symbolTableOutput.h symbolTableOutput.cpp lexer.h
	g++ -g -o build/symbolTableOutput.o -c symbolTableOutput.cpp

quadRuplesOutput.o: quadRuplesOutput.h quadRuplesOutput.cpp
	g++ -g -o build/quadRuplesOutput.o -c quadRuplesOutput.cpp

machineCodeOutput.o: machineCodeOutput.h machineCodeOutput.cpp
	g++ -g -o build/machineCodeOutput.o -c machineCodeOutput.cpp

parse_tree.o: parse_tree.h parse_tree.cpp
	g++ -g -o build/parse_tree.o -c parse_tree.cpp
