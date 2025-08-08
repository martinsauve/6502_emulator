all: build run

build: main.c
	cc main.c -o 6502

run:
	./6502

clean:
	rm 6502 dump.bin
