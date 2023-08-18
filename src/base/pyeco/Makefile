# Compiler and linker options
CC := gcc
CFLAGS := -Wall -Werror -fPIC -MMD
LDFLAGS := -lm -shared
LIBS := -lpython3.11

# Source files and object files
SRC := $(wildcard *.c)
OBJ := $(SRC:.c=.o)
DEP := $(SRC:.c=.d)

# Target shared library names
TARGET_LINUX := libpyeco.so
TARGET_MAC := libpyeco.dylib

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
