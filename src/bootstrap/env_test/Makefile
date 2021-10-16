ifeq ($(PYTHON),)
PYTHON := python3
endif

NAME = master.ebf


%.ebf: %.eco
	PYTHONPATH=../py2 $(PYTHON) -m ecosphere.bootstrapper $< $@

all: $(NAME)


clean:
	- $(RM) $(NAME)

re: clean all
