
.phony: all
all:
	$(MAKE) -C src/base/eve
	$(MAKE) -C src/base/ecore
	$(MAKE) -C src/eco/lib

.phony: clean
clean:
	$(MAKE) -C src/base/eve clean
	$(MAKE) -C src/base/ecore clean
	$(MAKE) -C src/eco/lib clean

.phony: run
run: all
	cd src/base/ecore; $(MAKE) run

.phony: re
re: clean run
