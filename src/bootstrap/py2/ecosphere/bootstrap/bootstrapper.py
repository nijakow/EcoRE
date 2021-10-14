import sys
import os.path
import pathlib

import ecosphere.parser.stream
import ecosphere.parser.tokenizer
import ecosphere.parser.parser
import ecosphere.compiler
import ecosphere.econnect


class FileLoader:

    def load_relative_path(self, path):
        return self._shared_info.load(self._path.joinpath(path).resolve())

    def parse_expressions(self):
        print('Loading', self._path.as_posix(), '...')
        s = ecosphere.parser.stream.StringStream(self._path.read_text())
        t = ecosphere.parser.tokenizer.Tokenizer(s)
        p = ecosphere.parser.parser.Parser(t)
        expressions = p.parse_expressions(ecosphere.parser.tokenizer.TokenType.EOF)
        return expressions
    
    def evaluate(self):
        expressions = self.parse_expressions()
        holder = dict()
        def callback(value):
            holder['value'] = value
        for expr in expressions:
            expr.evaluate(None, self, the_callback)
        return holder['value']

    def __init__(self, shared_info, path):
        self._shared_info = shared_info
        self._path = path

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
    loader = shared.load(srcfile)
    result = loader.evaluate()
    serializer = ecosphere.econnect.Serializer()
    serializer.write_object(result)
    print(serializer.finish())  # TODO: Write to file

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
