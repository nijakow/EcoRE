# Compiler and linker options
CC := gcc
CFLAGS := -Wall -Werror -fPIC -MMD $(shell pkg-config --cflags sdl2 SDL2_ttf SDL2_gfx SDL2_image zlib libffi)
LDFLAGS := -lm -shared $(shell pkg-config --cflags sdl2 SDL2_ttf SDL2_gfx SDL2_image zlib libffi)
LIBS := -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_gfx

# Source files and object files
SRC := $(wildcard *.c)
OBJ := $(SRC:.c=.o)
DEP := $(SRC:.c=.d)

# Target shared library names
TARGET_LINUX := libeve.so
TARGET_MAC := libeve.dylib

# Targets
all: linux macos

linux: $(TARGET_LINUX)

macos: $(TARGET_MAC)

$(TARGET_LINUX): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)

$(TARGET_MAC): $(OBJ)
	$(CC) $(LDFLAGS) -dynamiclib -o $@ $^ $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

-include $(DEP)

clean:
	rm -f $(OBJ) $(DEP) $(TARGET_LINUX) $(TARGET_MAC)
