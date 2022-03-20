ifeq ($(PYTHON),)
PYTHON := python3
endif

NAME = master.ebf
SOURCES = \
init.eco \
std/_.eco \
std/bool.eco \
std/clonable.eco \
std/defaultbehavior.eco \
std/globals.eco \
std/io.eco \
std/maybe.eco \
std/util.eco \
std/reflect/_.eco \
std/reflect/mirror.eco \
std/reflect/proxies/_.eco \
std/reflect/proxies/block.eco \
interfaces.eco \
lang/_.eco \
lang/eco/_.eco \
lang/eco/ast/ast.eco \
lang/eco/ast/construction.eco \
lang/eco/ast/visitor.eco \
lang/eco/compiler/_.eco \
lang/eco/compiler/bytecodes.eco \
lang/eco/compiler/codewriter.eco \
lang/eco/compiler/compiler.eco \
lang/eco/compiler/regalloc.eco \
lang/eco/compiler/scope.eco \
lang/eco/compiler/scoped_compiler.eco \
lang/eco/compiler/value_type.eco \
lang/eco/compiler/visitor.eco \
lang/eco/parser/Parser/objects/interface.eco \
lang/eco/parser/Parser/objects/molecule.eco \
lang/eco/parser/Parser/objects/simple.eco \
lang/eco/parser/Parser/args.eco \
lang/eco/parser/Parser/bars.eco \
lang/eco/parser/Parser/charchecks.eco \
lang/eco/parser/Parser/checks.eco \
lang/eco/parser/Parser/compounds.eco \
lang/eco/parser/Parser/expressions.eco \
lang/eco/parser/Parser/optionals.eco \
lang/eco/parser/Parser/send.eco \
lang/eco/parser/_.eco \
lang/eco/parser/reader.eco \
lang/eco/parser/shared.eco \
lang/eco/parser/tokenizer.eco \
master.eco


$(NAME): $(SOURCES) Makefile
	PYTHONPATH=../bootstrapper $(PYTHON) -m ecosphere.bootstrapper --compressed $(NAME) $(SOURCES)

all: $(NAME)


clean:
	- $(RM) $(NAME)

re: clean all
