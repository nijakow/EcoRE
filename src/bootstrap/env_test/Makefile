
BOOTSTRAPPER = ../py/bootstrapper.py

NAME = master.ebf


%.ebf: %.eco
	$(BOOTSTRAPPER) $< $@

all: $(NAME)


clean:
	- $(RM) $(NAME)

re: clean all
