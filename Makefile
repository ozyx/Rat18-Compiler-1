CC = g++
FLAGS = -std=c++14 -Wall -g
OBJS = main.o
NAME = program.exe

all: Program

Program: main.o
	$(CC) -o $(NAME) $(OBJS)

main.o: main.cpp
	$(CC) $(FLAGS) -c main.cpp -o main.o

clean:
	rm -v *.o $(NAME)