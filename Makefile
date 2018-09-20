CC = g++
FLAGS = -std=c++1y -Wall -g
OBJS = main.o Lexer.o
NAME = program.exe

all: Program

Program: Lexer.o main.o
	$(CC) -o $(NAME) $(OBJS)

main.o: main.cpp
	$(CC) $(FLAGS) -c main.cpp -o main.o

Lexer.o: Lexer.cpp
	$(CC) $(FLAGS) -c Lexer.cpp -o Lexer.o

clean:
	rm -v *.o $(NAME)