RELEASE ?= 0
SRC := ./src
ROMS := ./roms
EXECUTABLE_NAME := 6502

CC := gcc
DEBUG_CFLAGS := -Wall -Wextra -ggdb -pedantic -Werror -Wundef -fno-common
RELEASE_CFLAGS := -Wall -Wextra -O3 -flto

RAYLIB_RELEASE_URL := https://github.com/raysan5/raylib/releases/download/5.5/raylib-5.5_linux_amd64.tar.gz
RAYLIB_PATH := ./third_party/raylib
RAYLIB_INCLUDE := $(RAYLIB_PATH)/include
RAYLIB_LIB := $(RAYLIB_PATH)/lib

SOURCES := $(wildcard $(SRC)/*/*.c) $(wildcard $(SRC)/*.c)
OBJECTS := $(SOURCES:.c=.o)
DEPENDS := $(patsubst %.c,%.d, $(SOURCES))


ifeq ($(RELEASE),1)
CFLAGS := $(RELEASE_CFLAGS)
else
CFLAGS := $(DEBUG_CFLAGS)
endif

CFLAGS += -I$(RAYLIB_INCLUDE)
LDFLAGS := $(CFLAGS) -L$(RAYLIB_LIB)
LINKDED_LIBS := -lraylib -lm -lpthread -ldl

CFLAGS += -MMD -MP

VASM := vasm6502_oldstyle
VASMFLAGS := -Fbin -dotdir -esc -c02

ROMS_SOURCES := $(wildcard $(ROMS)/*.s) $(wildcard $(ROMS)/*/*.s)
ROMS_BIN := $(ROMS_SOURCES:.s=.bin)


.PHONY: raylib

all: build roms #run

$(RAYLIB_PATH)/.downloaded:
	@mkdir -p $(RAYLIB_PATH)
	@echo "downloading raylib..."
	wget $(RAYLIB_RELEASE_URL) -O $(RAYLIB_PATH).tar.gz
	tar -xzf $(RAYLIB_PATH).tar.gz -C $(RAYLIB_PATH) --strip-components=1
	rm $(RAYLIB_PATH).tar.gz
	touch $(RAYLIB_PATH)/.downloaded
	@echo "================================================================================"
	@echo "raylib successfully downloaded! Please add '$(RAYLIB_LIB)' to your LD_LIBRARY_PATH environment variable if necessary."
	@echo "================================================================================"

raylib: $(RAYLIB_PATH)/.downloaded


%.bin : %.s Makefile
	$(VASM) $(VASMFLAGS) $< -o $@

roms: $(ROMS_BIN)

-include $(DEPENDS)

%.o : %.c Makefile
	$(CC) $(CFLAGS) -c $< -o $@


build: raylib $(OBJECTS)
	$(CC) $(LDFLAGS) -o $(EXECUTABLE_NAME) $(OBJECTS) $(LINKDED_LIBS)

run:
	./$(EXECUTABLE_NAME) --gui

clean:
	rm -f $(DEPENDS)
	rm -f $(OBJECTS)
	rm -f $(EXECUTABLE_NAME)
	rm -f dump.bin

clean-roms:
	rm -f $(ROMS_BIN)

clean-raylib:
	rm -rf $(RAYLIB_PATH)

clean-all: clean clean-roms raylib

clean-build: clean-all build roms
