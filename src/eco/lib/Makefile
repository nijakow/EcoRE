ifeq ($(PYTHON),)
PYTHON := python3
endif

NAME = master.ebf
SOURCES = $(shell find . -name "*.eco" -not -name "master.eco") master.eco

$(NAME): $(SOURCES) Makefile
	PYTHONPATH=../../base/bootstrapper $(PYTHON) -m ecosphere.bootstrapper --binary --compressed $(NAME) $(SOURCES)

all: $(NAME)


clean:
	- $(RM) $(NAME)

re: clean
	$(MAKE) all
