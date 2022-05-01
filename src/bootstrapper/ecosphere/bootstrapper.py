import sys
import time
import pathlib
import os

import ecosphere.parser.stream
import ecosphere.parser.tokenizer
import ecosphere.parser.parser
import ecosphere.compiler
import ecosphere.econnect


class LabelStorage:

    def when_label_defined(self, label, callback):
        if label in self._label_values:
            callback(self._label_values[label])
            return
        if label not in self._callbacks:
            self._callbacks[label] = list()
        self._callbacks[label].append(callback)
    
    def define_label(self, label, value):
        if label in self._label_values:
            print('Warning: Redefining label \'', label, '\'!', sep='')
        self._label_values[label] = value
        if label in self._callbacks:
            for callback in self._callbacks[label]:
                callback(value)
            del self._callbacks[label]

    def __init__(self):
        self._label_values = dict()
        self._callbacks = dict()


class FileLoader:

    def load_file(self, path):
        parent = self._path.parents[0]
        return self._shared_info.load(parent.joinpath(path).resolve())

    def _parse_expressions(self):
        text = self._path.read_text()
        s = ecosphere.parser.stream.StringStream(text)
        t = ecosphere.parser.tokenizer.Tokenizer(s)
        p = ecosphere.parser.parser.Parser(t)
        expressions = p.parse()
        return expressions
    
    def evaluate(self):
        if self._evaluated:
            return self._evaluated
        if not self._expressions:
            self._expressions = self._parse_expressions()
        def callback(value):
            self._evaluated = value
        for expr in self._expressions:
            expr.evaluate(None, self, callback)
        return self._evaluated
    
    def compile(self):
        if not self._expressions:
            self._expressions = self._parse_expressions()
        return ecosphere.compiler.compile_thunk(self._expressions, self)
    
    def when_label_defined(self, label, callback):
        self._shared_info.get_label_storage().when_label_defined(label, callback)
    
    def define_label(self, label, value):
        self._shared_info.get_label_storage().define_label(label, value)

    def __init__(self, shared_info, path):
        self._shared_info = shared_info
        self._path = path
        self._expressions = None
        self._evaluated = None

class SharedBootstrappingInfo:

    def load(self, path):
        path = path.resolve()
        as_posix = path.as_posix()
        if as_posix not in self._loaded_files:
            loader = FileLoader(self, path)
            self._loaded_files[as_posix] = loader
            return loader
        else:
            return self._loaded_files[as_posix]
    
    def get_label_storage(self):
        return self._label_storage

    def __init__(self):
        self._loaded_files = dict()
        self._label_storage = LabelStorage()


def banner(compressed=False):
    print('''
     ______          _____  ______   _                 _    ____  
    |  ____|        |  __ \|  ____| | |               | |  / /\ \ 
    | |__   ___ ___ | |__) | |__    | |__   ___   ___ | |_| |  | |
    |  __| / __/ _ \|  _  /|  __|   | '_ \ / _ \ / _ \| __| |  | |
    | |___| (_| (_) | | \ \| |____ _| |_) | (_) | (_) | |_| |  | |
    |______\___\___/|_|  \_\______(_)_.__/ \___/ \___/ \__| |  | |
                                                           \_\/_/ 

''')
    if compressed:
        print('  Compiling (compressed mode active)...')
    else:
        print('  Compiling ...')
    print()

def convert(f, bts):
    print('#ifndef ECO_CONFIG_COMPILE_WITH_INTEGRATED_BINARY', file=f)
    print('# define ECO_CONFIG_COMPILE_WITH_INTEGRATED_BINARY', file=f)
    print('#endif', file=f)
    print(file=f)
    print('char Eco_DEFAULT_IMAGE[] = ', file=f)
    print('{', file=f)
    print('    ', end='', file=f)
    i = 0
    for b in bts:
        print('0x{:02x}, '.format(b), end='', file=f)
        i += 1
        if i % 8 == 0:
            print(file=f)
            print('    ', end='', file=f)
    if i % 8 != 0:
        print(file=f)
    print('};', file=f)
    print(file=f)
    print('const unsigned int Eco_DEFAULT_IMAGE_LENGTH =', i, ';', file=f)

def main(binfile, srcfiles, as_c=False, compressed=False):
    banner(compressed)
    shared = SharedBootstrappingInfo()
    result = None
    rotates = " ⠁⠉⠙⠚⠖⠦⠤⠠"
    i = int(time.time() * 10)
    for srcfile in srcfiles:
        print('\033[1A\033[2K', rotates[(int(time.time() * 10) - i) % len(rotates)], ' Loading ', srcfile, '...', sep='')
        loader = shared.load(pathlib.Path(srcfile))
        result = loader.compile()
    serializer = ecosphere.econnect.Serializer()
    serializer.write_object(result)
    mode = 'wb'
    if as_c:
       mode = 'w'
    with open(binfile, mode) as out:
        if compressed:
            serialized = serializer.finish_compressed()
        else:
            serialized = serializer.finish()
        print('  Writing', len(serialized), 'bytes to', binfile, '...')
        if as_c:
            convert(out, serialized)
        else:
            out.write(serialized)


if __name__ == '__main__':
    index = 1
    as_c = True
    compressed = False
    while True:
        if sys.argv[index] == '--binary':
            as_c = False
            index += 1
        elif sys.argv[index] == '--compressed':
            compressed = True
            index += 1
        else:
            break
    ofile = sys.argv[index]
    ifiles = sys.argv[(index + 1):]
    main(ofile, ifiles, as_c, compressed)

