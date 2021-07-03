
ifdef OS
  CC = mingw32-gcc
  RM = erase
  MAKE = mingw32-make
else
  RM = rm
endif

EXECUTABLE = ./ecore.exe
PROJECT_ROOT_DIR = $(CURDIR)/..

SOURCES = \
eco.c \
objects/type.c \
objects/object.c \
objects/key.c \
objects/group.c \
objects/code.c \
objects/closure.c \
vm/memory/memory.c \
vm/memory/gc/gc_state.c \
vm/memory/gc/gc.c \
vm/core/environment.c \
vm/core/frame.c \
vm/core/send.c \
vm/core/interpreter.c \
vm/fiber.c \
vm/vm.c \
io/net/net.c \
io/net/scheduler.c \
io/net/socket.c \
io/econnect/econnect.c \
io/econnect/callback.c \
io/econnect/instance.c \
io/econnect/reader.c \
io/econnect/parser.c \
io/econnect/net/connection.c \
main.c

OBJECTS = $(SOURCES:.c=.o)
DEPENDS = $(patsubst %.c,%.d,$(SOURCES))

LDFLAGS =
CFLAGS = -g -I $(PROJECT_ROOT_DIR)


%.o: %.c
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $(EXECUTABLE)

all: $(EXECUTABLE)


.phony: run
run: $(EXECUTABLE)
	$(EXECUTABLE)

.phony: debug
debug: $(EXECUTABLE)
	gdb $(EXECUTABLE)

.phony: test
test: $(EXECUTABLE)
	valgrind --leak-check=full $(EXECUTABLE)

.phony: clean
clean:
	- $(RM) $(EXECUTABLE) $(OBJECTS) $(DEPENDS)

.phony: re
re: clean all

-include $(DEPENDS)
