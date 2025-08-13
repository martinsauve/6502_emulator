SRC := ./src
ROMS := ./roms
EXECUTABLE_NAME := 6502

CC := gcc
CFLAGS := -Wall -Wextra -ggdb -O3

VASM := vasm6502_oldstyle
VASMFLAGS := -Fbin -dotdir -esc

SOURCES := $(wildcard $(SRC)/*.c)
TESTS := $(wildcard $(SRC)/tests/*.c)
OBJECTS := $(SOURCES:.c=.o)
OBJECTS_TESTS := $(TESTS:.c=.o)
DEPENDS := $(patsubst %.c,%.d, $(SOURCES))
DEPENDS_TESTS := $(patsubst %.c,%.d, $(TESTS))

ROMS_SOURCES := $(wildcard $(ROMS)/*.s)
ROMS_BIN := $(ROMS_SOURCES:.s=.bin)


all: build-tests roms run


%.bin : %.s Makefile
	$(VASM) $(VASMFLAGS) $< -o $@

roms: $(ROMS_BIN)

-include $(DEPENDS) $(DEPENDS_TESTS)

%.o : %.c Makefile
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@


build: $(OBJECTS)
	$(CC) $(CFLAGS) -o $(EXECUTABLE_NAME) $^

build-tests: $(OBJECTS) $(TESTS:.c=.o)
	$(CC) $(CFLAGS) -o $(EXECUTABLE_NAME) $^


run:
	./$(EXECUTABLE_NAME)

clean:
	rm -f $(DEPENDS) $(DEPENDS_TESTS)
	rm -f $(OBJECTS)
	rm -f $(OBJECTS_TESTS)
	rm -f $(EXECUTABLE_NAME)
	rm -f dump.bin

clean-roms:

	rm -f $(ROMS_BIN)

clean-build: clean clean-roms build-tests roms
