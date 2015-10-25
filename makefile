# Makefile super op

all: first.o everything exe

first.o: first.cpp
	g++ -c first.cpp
	
everything:
	g++ -o game first.o -lsfml-graphics -lsfml-window -lsfml-system -lsfml-network
		
exe:
	./game
	
clear:
	rm *.o game
