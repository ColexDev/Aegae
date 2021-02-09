all: main

main: main.cpp
	g++ -lncurses -lmenu main.cpp -o main
