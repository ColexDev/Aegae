all: main
main: main.cpp
	g++ -I/usr/local/include main.cpp get.cpp length.cpp file.cpp -L/usr/local/lib -Wl,-R/usr/local/lib -lncurses -o Corcyra
