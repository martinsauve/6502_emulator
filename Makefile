SRC := ./src
SOURCES := $(wildcard $(SRC)/*.c)
HEADERS := $(wildcard $(SRC)/*.h)
ARTIFACTS := $(SOURCES:.c=.o)
EXECUTABLE_NAME := 6502


all: build run

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

build: $(ARTIFACTS)
	$(CC) -o $(EXECUTABLE_NAME) $^



run:
	./$(EXECUTABLE_NAME)

clean:
	rm -f $(ARTIFACTS) $(EXECUTABLE_NAME)

clean-build: clean build
