ifeq ($(PYTHON),)
PYTHON := python3
endif

NAME = master.ebf
SOURCES = \
std/_.eco \
std/bool.eco \
std/clonable.eco \
std/defaultbehavior.eco \
std/enum.eco \
std/globals.eco \
std/maybe.eco \
std/module.eco \
std/modules.eco \
std/singleton.eco \
std/try.eco \
std/io/_.eco \
std/io/fdbytereader.eco \
std/io/fdbytewriter.eco \
std/io/fdsegmentreader.eco \
std/io/filedescriptor.eco \
std/io/portinput.eco \
std/io/portio.eco \
std/io/portoutput.eco \
std/io/stringwriter.eco \
std/io/writer.eco \
std/os/_.eco \
std/os/files/_.eco \
std/ffi/_.eco \
std/reflect/_.eco \
std/reflect/mirror.eco \
std/reflect/slot.eco \
std/reflect/proxies/_.eco \
std/reflect/proxies/array.eco \
std/reflect/proxies/blob.eco \
std/reflect/proxies/block.eco \
std/reflect/proxies/char.eco \
std/reflect/proxies/code.eco \
std/reflect/proxies/float.eco \
std/reflect/proxies/int.eco \
std/reflect/proxies/interface.eco \
std/reflect/proxies/key.eco \
std/reflect/proxies/string.eco \
std/reflect/proxies/type.eco \
std/reflect/proxies/ffi/ffifunc.eco \
std/reflect/proxies/ffi/ffiobject.eco \
std/reflect/proxies/ffi/ffitype.eco \
std/reflect/proxies/ffi/ffilib.eco \
std/sys/_.eco \
std/sys/builtins.eco \
std/text/_.eco \
std/text/parsing/_.eco \
std/text/parsing/number.eco \
std/text/utf8/_.eco \
std/text/utf8/utf8iterator.eco \
std/text/writing/_.eco \
std/text/writing/numberwriter.eco \
std/util/_.eco \
std/util/comparators/_.eco \
std/util/iterators/segmented/_.eco \
std/util/iterators/_.eco \
std/util/collector.eco \
std/util/map.eco \
std/util/mutablepair.eco \
std/util/queue.eco \
std/util/range.eco \
std/util/set.eco \
std/util/stack.eco \
std/util/vectors.eco \
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
lang/eco/parser/shared.eco \
lang/eco/parser/tokenizer.eco \
interfaces.eco \
master.eco


$(NAME): $(SOURCES) Makefile
	PYTHONPATH=../bootstrapper $(PYTHON) -m ecosphere.bootstrapper --binary --compressed $(NAME) $(SOURCES)

all: $(NAME)


clean:
	- $(RM) $(NAME)

re: clean
	$(MAKE) all
