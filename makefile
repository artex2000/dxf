SRC=main.c

all: 
	gcc -o dxf `pkg-config --libs --cflags gtk+-2.0` -lm $(SRC)
