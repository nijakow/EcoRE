
.phony: all
all:
	$(MAKE) -C src/base/eve
	$(MAKE) -C src/base/ecore

.phony: run
run: all
	$(MAKE) -C src/base/ecore run

.phony: clean
clean:
	$(MAKE) -C src/base/eve clean
	$(MAKE) -C src/base/ecore clean

.phony: re
re: clean run
