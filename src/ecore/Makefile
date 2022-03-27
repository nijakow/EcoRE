
ifdef OS
  CC = mingw32-gcc
  RM = erase
  MAKE = mingw32-make
else
  RM = rm
endif

EXECUTABLE = ./ecore.app

SOURCES = \
memory/memory.c \
objects/base/object.c \
objects/pair/pair.c \
objects/symbol/symbol.c \
utils/string.c \
main.c

OBJECTS = $(patsubst %.c,%.o,$(SOURCES))
DEPENDS = $(patsubst %.c,%.d,$(SOURCES))

LDFLAGS = 
CFLAGS = -I.. -Wall -Wextra -Werror -Wno-unused-parameter -Wno-unused-label -g -Ofast


%.o: %.c
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $(EXECUTABLE)

all: $(EXECUTABLE)

compile_commands.json:
	bear -- make all

.phony: run
run: $(EXECUTABLE)
	$(EXECUTABLE)

.phony: debug
debug: $(EXECUTABLE)
	gdb $(EXECUTABLE)

.phony: leakcheck
leakcheck: $(EXECUTABLE)
	valgrind --leak-check=full --show-leak-kinds=all $(EXECUTABLE)

.phony: clean
clean:
	- $(RM) $(EXECUTABLE) $(OBJECTS) $(DEPENDS)

.phony: re
re: clean all

-include $(DEPENDS)
