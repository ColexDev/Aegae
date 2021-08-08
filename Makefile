all: main
main: main.cpp
	g++ -I/usr/local/include main.cpp get.cpp budget.cpp length.cpp file.cpp table.cpp utils.cpp -L/usr/local/lib -Wl,-R/usr/local/lib -lncurses -o Corcyra
