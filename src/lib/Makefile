ifeq ($(PYTHON),)
PYTHON := python3
endif

NAME = master.ebf
SOURCES = \
init.eco \
std/std.eco \
std/io.eco \
std/util.eco \
std/reflect.eco \
interfaces.eco \
lang/parser/tokenizer.eco \
lang/parser/parser.eco \
master.eco


$(NAME): $(SOURCES)
	PYTHONPATH=../bootstrapper $(PYTHON) -m ecosphere.bootstrapper $(NAME) $(SOURCES)

all: $(NAME)


clean:
	- $(RM) $(NAME)

re: clean all
