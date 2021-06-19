
ifdef OS
  CC = mingw32-gcc
  RM = erase
  MAKE = mingw32-make
else
  RM = rm
endif

EXECUTABLE = ./ecore.exe

SOURCES = \
objects/type.c \
objects/object.c \
objects/group.c \
objects/code.c \
objects/closure.c \
vm/memory/memory.c \
vm/memory/gc_state.c \
vm/memory/gc.c \
vm/environment.c \
vm/frame.c \
vm/fiber.c \
vm/send.c \
vm/interpreter.c \
vm/vm.c \
eco.c \
main.c

OBJECTS = $(SOURCES:.c=.o)
DEPENDS = $(patsubst %.c,%.d,$(SOURCES))

LDFLAGS =
CFLAGS =


%.o: %.c
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $(EXECUTABLE)

all: $(EXECUTABLE)


.phony: run
run: all
	$(EXECUTABLE)

.phony: clean
clean:
	- $(RM) $(EXECUTABLE) $(OBJECTS) $(DEPENDS)

-include $(DEPENDS)
