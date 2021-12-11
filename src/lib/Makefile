ifeq ($(PYTHON),)
PYTHON := python3
endif

NAME = master.ebf
SOURCES = \
init.eco \
std/std.eco \
std/io.eco \
std/util.eco \
master.eco


$(NAME): $(SOURCES)
	PYTHONPATH=../bootstrapper $(PYTHON) -m ecosphere.bootstrapper $(NAME) $(SOURCES)

all: $(NAME)


clean:
	- $(RM) $(NAME)

re: clean all
