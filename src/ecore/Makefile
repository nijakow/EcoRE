
ifdef OS
  CC = mingw32-gcc
  RM = erase
  MAKE = mingw32-make
else
  RM = rm
endif

EXECUTABLE = ./ecore.app
BOOTSTRAP_ENVIRONMENT = ../lib
BOOTSTRAP_MASTER      = $(BOOTSTRAP_ENVIRONMENT)/master.ebf

SOURCES = \
eco.c \
objects/objects.c \
objects/base/type.c \
objects/base/typecore.c \
objects/base/object.c \
objects/molecule/molecule.c \
objects/molecule/object_payload.c \
objects/molecule/object_slots.c \
objects/io/port.c \
objects/misc/array/array.c \
objects/misc/blob/blob.c \
objects/misc/key/key.c \
objects/misc/string/string.c \
objects/pseudo/character.c \
objects/pseudo/integer.c \
objects/vm/code/code.c \
objects/vm/code/closure.c \
objects/vm/interface/interface.c \
objects/ffi/ffitype.c \
objects/ffi/ffifunc.c \
vm/builtins/builtins.c \
vm/builtins/core/array.c \
vm/builtins/core/blob.c \
vm/builtins/core/clone.c \
vm/builtins/core/code.c \
vm/builtins/core/flow.c \
vm/builtins/core/init.c \
vm/builtins/core/io.c \
vm/builtins/core/is.c \
vm/builtins/core/key.c \
vm/builtins/core/math.c \
vm/builtins/core/port.c \
vm/builtins/core/reflection.c \
vm/builtins/core/slots.c \
vm/builtins/core/string.c \
vm/memory/memory.c \
vm/memory/arena.c \
vm/memory/gc/gc_state.c \
vm/memory/gc/gc.c \
vm/core/frame.c \
vm/core/send.c \
vm/core/clone.c \
vm/core/interpreter.c \
vm/fiber/fiber.c \
vm/fiber/sched.c \
vm/scheduler.c \
vm/vm.c \
io/logging/log.c \
io/econnect/econnect.c \
io/econnect/instance.c \
io/econnect/reader/callback.c \
io/econnect/reader/parser.c \
io/econnect/reader/reader.c \
io/econnect/reader/result.c \
io/econnect/reader/builtins/builtins.c \
io/econnect/reader/builtins/compression/gz.c \
io/econnect/reader/builtins/protocol/protocol.c \
io/econnect/reader/builtins/objects/array.c \
io/econnect/reader/builtins/objects/interface.c \
io/econnect/reader/builtins/objects/ints.c \
io/econnect/reader/builtins/objects/object.c \
io/econnect/reader/builtins/objects/object_body.c \
io/econnect/reader/builtins/objects/key.c \
io/econnect/reader/builtins/objects/string.c \
io/econnect/reader/builtins/objects/code.c \
io/econnect/reader/builtins/id/id.c \
io/econnect/reader/builtins/flow/flow.c \
io/econnect/file/file.c \
util/gz.c \
util/memory.c \
util/utf8.c \
main.c

OBJECTS = $(patsubst %.c,%.o,$(SOURCES))
DEPENDS = $(patsubst %.c,%.d,$(SOURCES))

LDFLAGS = -lz -lffi
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
	$(MAKE) -C $(BOOTSTRAP_ENVIRONMENT)
	$(EXECUTABLE) $(BOOTSTRAP_MASTER)

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
