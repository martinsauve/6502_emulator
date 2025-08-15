SRC := ./src
ROMS := ./roms
EXECUTABLE_NAME := 6502

CC := gcc
CFLAGS := -Wall -Wextra -ggdb

VASM := vasm6502_oldstyle
VASMFLAGS := -Fbin -dotdir -esc -c02

SOURCES := $(wildcard $(SRC)/*/*.c) $(wildcard $(SRC)/*.c)
OBJECTS := $(SOURCES:.c=.o)
DEPENDS := $(patsubst %.c,%.d, $(SOURCES))

ROMS_SOURCES := $(wildcard $(ROMS)/*.s) $(wildcard $(ROMS)/*/*.s)
ROMS_BIN := $(ROMS_SOURCES:.s=.bin)


all: build roms run


%.bin : %.s Makefile
	$(VASM) $(VASMFLAGS) $< -o $@

roms: $(ROMS_BIN)

-include $(DEPENDS)

%.o : %.c Makefile
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@


build: $(OBJECTS)
	$(CC) $(CFLAGS) -o $(EXECUTABLE_NAME) $^

run:
	./$(EXECUTABLE_NAME)

clean:
	rm -f $(DEPENDS)
	rm -f $(OBJECTS)
	rm -f $(EXECUTABLE_NAME)
	rm -f dump.bin

clean-roms:

	rm -f $(ROMS_BIN)

clean-all: clean clean-roms

clean-build: clean clean-roms build roms
