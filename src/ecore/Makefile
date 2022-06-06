
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
objects/base/type/copying.c \
objects/base/type/gc.c \
objects/base/type/interface.c \
objects/base/type/sending.c \
objects/base/type/slot_info.c \
objects/base/type/slot.c \
objects/base/type/type.c \
objects/base/type/typecore.c \
objects/base/type/typelist.c \
objects/base/object/object.c \
objects/base/object/gc.c \
objects/base/object/send.c \
objects/base/molecule/molecule.c \
objects/base/molecule/object_payload.c \
objects/base/molecule/object_slots.c \
objects/base/type_transfer.c \
objects/misc/array/array.c \
objects/misc/blob/blob.c \
objects/misc/key/key.c \
objects/misc/string/string.c \
objects/pseudo/character.c \
objects/pseudo/float.c \
objects/pseudo/integer.c \
objects/vm/code/code.c \
objects/vm/code/closure.c \
objects/vm/interface/interface.c \
objects/vm/ffi/ffitype.c \
objects/vm/ffi/ffiobject.c \
objects/vm/ffi/ffifunc.c \
objects/vm/ffi/ffilib.c \
vm/builtins/builtins.c \
vm/builtins/core/array.c \
vm/builtins/core/blob.c \
vm/builtins/core/clone.c \
vm/builtins/core/code.c \
vm/builtins/core/ffi.c \
vm/builtins/core/flow.c \
vm/builtins/core/init.c \
vm/builtins/core/io.c \
vm/builtins/core/is.c \
vm/builtins/core/key.c \
vm/builtins/core/math.c \
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
vm/fiber/gc.c \
vm/fiber/sched.c \
vm/fiber/stackops.c \
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
util/libc.c \
util/memory.c \
util/utf8.c \
util/dwarf/dwarf.c \
main.c

OBJECTS = $(patsubst %.c,%.o,$(SOURCES))
DEPENDS = $(patsubst %.c,%.d,$(SOURCES))

LDFLAGS = -lz -lffi -ldl -lm
CFLAGS = -I.. -I/opt/homebrew/Cellar/libffi/3.4.2/include/ -Wall -Wextra -Werror -Wno-strict-aliasing -Wno-unused-result -Wno-unused-parameter -Wno-unused-label -g -Ofast -DECO_LATEST_GIT_COMMIT="\"$(shell git rev-parse --short=12 HEAD)\""

%.o: %.c
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $(EXECUTABLE)

all: $(EXECUTABLE)

compile_commands.json:
	bear -- make all

.phony: run
run: all
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
	- $(MAKE) -C ../lib/ clean
	- $(RM) $(EXECUTABLE) $(OBJECTS) $(DEPENDS)

.phony: re
re: clean all

-include $(DEPENDS)
