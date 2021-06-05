
ifdef OS
  CC = mingw32-gcc
  RM = erase
  MAKE = mingw32-make
else
  RM = rm
endif

EXECUTABLE = ./ecore.exe

SOURCES = \
vm/environment.c \
vm/fiber.c \
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
