CC = g++
FLAGS = -std=c++1y -Wall -g
OBJS = main.o Lexer.o SymbolTable.o SyntaxAnalyzer.o
NAME = program.exe

all: Program

Program: Lexer.o SymbolTable.o SyntaxAnalyzer.o main.o
	$(CC) -o $(NAME) $(OBJS)

main.o: main.cpp
	$(CC) $(FLAGS) -c main.cpp -o main.o

SyntaxAnalyzer.o: SyntaxAnalyzer.cpp
	$(CC) $(FLAGS) -c SyntaxAnalyzer.cpp -o SyntaxAnalyzer.o

SymbolTable.o: SymbolTable.cpp
	$(CC) $(FLAGS) -c SymbolTable.cpp -o SymbolTable.o

Lexer.o: Lexer.cpp
	$(CC) $(FLAGS) -c Lexer.cpp -o Lexer.o

clean:
	rm -v *.o $(NAME)