SRC := ./src
EXECUTABLE_NAME := 6502
CC := gcc
CFLAGS := -Wall -Wextra

SOURCES := $(wildcard $(SRC)/*.c)
TESTS := $(wildcard $(SRC)/tests/*.c)
OBJECTS := $(SOURCES:.c=.o)
OBJECTS_TESTS := $(TESTS:.c=.o)
DEPENDS := $(patsubst %.c,%.d, $(SOURCES))
DEPENDS_TESTS := $(patsubst %.c,%.d, $(TESTS))


all: build-tests run

-include $(DEPENDS) $(DEPENDS_TESTS)

%.o : %.c Makefile
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@


build: $(OBJECTS)
	$(CC) -o $(EXECUTABLE_NAME) $^

build-tests: $(OBJECTS) $(TESTS:.c=.o)
	$(CC) -o $(EXECUTABLE_NAME) $^


run:
	./$(EXECUTABLE_NAME)

clean:
	rm -f $(DEPENDS) $(DEPENDS_TESTS)
	rm -f $(OBJECTS)
	rm -f $(OBJECTS_TESTS)
	rm -f $(EXECUTABLE_NAME)
	rm -f dump.bin

clean-build: clean build-tests
