SRC=main.c parser.c list.c

all: 
	gcc -o dxf `pkg-config --libs --cflags glib-2.0 gtk+-2.0` -lm $(SRC)
