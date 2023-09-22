# Compiler and linker options
CC := gcc
CFLAGS := -Wall -Werror -fPIC -MMD -I/opt/homebrew/Cellar/sdl2/2.28.3/include -I/opt/homebrew/Cellar/sdl2/2.28.3/include/SDL2 -I/opt/homebrew/Cellar/sdl2_ttf/2.20.2/include -I/opt/homebrew/Cellar/sdl2_image/2.6.3_2/include
LDFLAGS := -lm -shared -L/opt/homebrew/Cellar/sdl2/2.28.3/lib -L/opt/homebrew/Cellar/sdl2_ttf/2.20.2/lib -L/opt/homebrew/Cellar/sdl2_image/2.6.3_2/lib
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
