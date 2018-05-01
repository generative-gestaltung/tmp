LDFLAGS = -L/usr/local/lib

main: patch.o patchSEQ.o patchLFO.o matrix.o dac.o main.o
	gcc patch.c patchSEQ.c patchLFO.c matrix.c dac.c main.c -O0 -o main -lwiringPi -lwiringPiDev -lbcm2835 -pthread -lm
