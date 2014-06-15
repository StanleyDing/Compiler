all: main.cpp grammar.o hash.o set.o lltable.o
	g++ -g main.cpp build/*

grammar.o: grammar.h grammar.cpp
	g++ -g -o build/grammar.o -c grammar.cpp

hash.o: hash.h hash.cpp
	g++ -g -o build/hash.o -c hash.cpp

set.o: set.h set.cpp
	g++ -g -o build/set.o -c set.cpp

lltable.o: lltable.h lltable.cpp
	g++ -g -o build/lltable.o -c lltable.cpp
