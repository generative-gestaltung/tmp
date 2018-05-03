LDFLAGS = -L/usr/local/lib

main: patch.o patchSEQ0.o patchSEQ1.o patchLFO.o matrix.o dac.o main.o
	gcc patch.c patchSEQ0.c patchSEQ1.c patchLFO.c matrix.c dac.c main.c -O0 -o main -lwiringPi -lwiringPiDev -lbcm2835 -pthread -lm
