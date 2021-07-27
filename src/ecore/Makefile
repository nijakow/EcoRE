
ifdef OS
  CC = mingw32-gcc
  RM = erase
  MAKE = mingw32-make
else
  RM = rm
endif

EXECUTABLE = ./ecore.exe
BOOTSTRAP_ENVIRONMENT = ../bootstrap/env_test
BOOTSTRAP_MASTER      = $(BOOTSTRAP_ENVIRONMENT)/master.ebf


SOURCES = \
eco.c \
objects/objects.c \
objects/base/type.c \
objects/base/typecore.c \
objects/base/object.c \
objects/base/object_payload.c \
objects/base/object_slots.c \
objects/group/group.c \
objects/misc/key/key.c \
objects/vm/code/code.c \
objects/vm/code/closure.c \
vm/memory/memory.c \
vm/memory/gc/gc_state.c \
vm/memory/gc/gc.c \
vm/core/environment.c \
vm/core/frame.c \
vm/core/send.c \
vm/core/interpreter.c \
vm/fiber.c \
vm/vm.c \
io/logging/log.c \
io/net/net.c \
io/net/scheduler.c \
io/net/socket.c \
io/econnect/econnect.c \
io/econnect/instance.c \
io/econnect/reader/callback.c \
io/econnect/reader/parser.c \
io/econnect/reader/reader.c \
io/econnect/reader/result.c \
io/econnect/reader/builtins/builtins.c \
io/econnect/reader/builtins/protocol/protocol.c \
io/econnect/reader/builtins/objects/object.c \
io/econnect/reader/builtins/objects/object_body.c \
io/econnect/reader/builtins/objects/key.c \
io/econnect/reader/builtins/objects/code.c \
io/econnect/reader/builtins/id/id.c \
io/econnect/reader/builtins/flow/flow.c \
io/econnect/file/file.c \
io/econnect/net/connection.c \
main.c

OBJECTS = $(SOURCES:.c=.o)
DEPENDS = $(patsubst %.c,%.d,$(SOURCES))

LDFLAGS =
CFLAGS = -Wall -Wextra -Werror -Wno-unused-parameter -g


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

.phony: leakcheck
leakcheck: $(EXECUTABLE)
	valgrind --leak-check=full --show-leak-kinds=all $(EXECUTABLE)

.phony: clean
clean:
	- $(RM) $(EXECUTABLE) $(OBJECTS) $(DEPENDS)

.phony: re
re: clean all

.phony: test
test: $(EXECUTABLE)
	$(EXECUTABLE) '$(BOOTSTRAP_MASTER)'

-include $(DEPENDS)
