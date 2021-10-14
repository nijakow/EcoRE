import sys
import os.path
import pathlib

import ecosphere.parser.stream
import ecosphere.parser.tokenizer
import ecosphere.parser.parser
import ecosphere.compiler
import ecosphere.econnect


class FileLoader:

    def load_file(self, path):
        parent = self._path.parents[0]
        return self._shared_info.load(parent.joinpath(path).resolve())

    def _parse_expressions(self):
        print('Loading', self._path.resolve().as_posix(), '...')
        text = self._path.read_text()
        s = ecosphere.parser.stream.StringStream(text)
        t = ecosphere.parser.tokenizer.Tokenizer(s)
        p = ecosphere.parser.parser.Parser(t)
        expressions = p.parse_expressions(ecosphere.parser.tokenizer.TokenType.EOF)
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

    def __init__(self, shared_info, path):
        self._shared_info = shared_info
        self._path = path
        self._expressions = None
        self._evaluated = None
        self._label_values = dict()
        self._callbacks = dict()

class SharedBootstrappingInfo:

    def load(self, path):
        as_posix = path.as_posix()
        if as_posix not in self._loaded_files:
            loader = FileLoader(self, path)
            self._loaded_files[as_posix] = loader
            return loader
        else:
            return self._loaded_files[as_posix]

    def __init__(self):
        self._loaded_files = dict()


def main(srcfile, binfile):
    shared = SharedBootstrappingInfo()
    loader = shared.load(pathlib.Path(srcfile))
    result = loader.evaluate()
    serializer = ecosphere.econnect.Serializer()
    serializer.write_object(result)
    with open(binfile, 'wb') as out:
        out.write(serializer.finish())

if __name__ == '__main__':
    main(sys.argv[1], sys.argv[2])


#def test(text):
#    s = ecosphere.parser.stream.StringStream(text)
#    t = ecosphere.parser.tokenizer.Tokenizer(s)
#    p = ecosphere.parser.parser.Parser(t)
#    expressions = p.parse_expressions(ecosphere.parser.tokenizer.TokenType.EOF)
#    holder = dict()
#    def the_callback(value):
#        holder['value'] = value
#    for expression in expressions:
#        expression.evaluate(None, None, the_callback)
#    value = holder['value']
#    print('Result:', value)
#    serializer = ecosphere.econnect.Serializer()
#    serializer.write_object(value)
#    print(serializer.finish())


#if __name__ == '__main__':
#    test('''
#    \{
#        "This is a comment!"
#        x = 5.
#        [int] y = 101.
#        with z = 7.
#        with* zz = 42.
#        with* [int] zzz = 42.
#        [any] routine => hey.
#        square: [int] x => x * x.
#        [int] f([int] x, [int] y) => (x * x) + (2 * y).
#        test => (
#            hello world.
#            this is a test.
#            [ :y => y ].
#            [ :x => y ].
#            [ :x => x <- a ].
#            [ | x <- 5 | x ].
#            y <- a.
#        ).
#    \}.
#    ''')
