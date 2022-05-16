
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

SOURCES = $(shell find . -name "*.c")

OBJECTS = $(patsubst %.c,%.o,$(SOURCES))
DEPENDS = $(patsubst %.c,%.d,$(SOURCES))

LDFLAGS = 
CFLAGS = -Wall -Wextra -Werror -Wno-strict-aliasing -Wno-unused-result -Wno-unused-parameter -Wno-unused-label -g -Ofast -DECO_LATEST_GIT_COMMIT="\"$(shell git rev-parse --short=12 HEAD)\""

%.o: %.c
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $(EXECUTABLE)

all: $(EXECUTABLE)

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
