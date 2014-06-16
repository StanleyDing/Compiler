main:main.o quadRuplesOutput.o
	g++ -o main main.o quadRuplesOutput.o
main.o:main.cpp
	g++ -c main.cpp
quadRuplesOutput.o:quadRuplesOutput.o
	g++ -o build/quadRuplesOutput.o -c quadRuplesOutput.cpp
