import ecosphere.parser.stream
import ecosphere.parser.tokenizer
import ecosphere.parser.parser

from pprint import pprint


def test(text):
    s = ecosphere.parser.stream.StringStream(text)
    t = ecosphere.parser.tokenizer.Tokenizer(s)
    p = ecosphere.parser.parser.Parser(t)
    for e in p.parse_expressions(ecosphere.parser.tokenizer.TokenType.EOF):
        print(type(e))
        pprint(e.__dict__)
        print()


if __name__ == '__main__':
    test('''
    hello world.
    this is a test.
    [ :x => y ].
    y <- a.
    \{
        "This is a comment!"
        x = 5.
        [int] y = 101.
        with z = foo.
        with* zz = foo.
        with* [int] zzz = 42.
        [any] routine => hey.
        square: [int] x => x * x.
        [int] f([int] x, [int] y) => (x * x) + (2 * y).
    \}.
    #=hello: \{ x = 5 \}.
    #~hello~.
    ''')
