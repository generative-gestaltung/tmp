LDFLAGS = -L/usr/local/lib

main: patch.o matrix.o dac.o main.o
	gcc patch.c matrix.c dac.c main.c -O0 -o main -lwiringPi -lwiringPiDev -lbcm2835 -pthread -lm
