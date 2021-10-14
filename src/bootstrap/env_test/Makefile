ifeq ($(PYTHON),)
PYTHON := python3
endif

NAME = test_v2.ebf


%.ebf: %.eco
	PYTHONPATH=../py2 $(PYTHON) -m ecosphere.bootstrapper $< $@

all: $(NAME)


clean:
	- $(RM) $(NAME)

re: clean all
