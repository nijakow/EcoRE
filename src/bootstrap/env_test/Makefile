
BOOTSTRAPPER = ../py/ecosphere/bootstrap.sh

NAME = master.ebf


%.ebf: %.eco
	$(BOOTSTRAPPER) $< $@

all: $(NAME)


clean:
	- $(RM) $(NAME)

re: clean all
