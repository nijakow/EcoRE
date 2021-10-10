import ecosphere.parser.stream
import ecosphere.parser.tokenizer
import ecosphere.parser.parser
import ecosphere.compiler
import ecosphere.econnect

from pprint import pprint


def test(text):
    s = ecosphere.parser.stream.StringStream(text)
    t = ecosphere.parser.tokenizer.Tokenizer(s)
    p = ecosphere.parser.parser.Parser(t)
    expression = p.parse_expression()
    holder = dict()
    def the_callback(value):
        holder['value'] = value    
    expression.evaluate(None, None, the_callback)
    value = holder['value']
    print('Result:', value)
    serializer = ecosphere.econnect.Serializer()
    serializer.write_object(value)
    print(serializer.finish())


if __name__ == '__main__':
    test('''
    \{
        "This is a comment!"
        x = 5.
        [int] y = 101.
        with z = 7.
        with* zz = 42.
        with* [int] zzz = 42.
        [any] routine => hey.
        square: [int] x => x * x.
        [int] f([int] x, [int] y) => (x * x) + (2 * y).
        test => (
            hello world.
            this is a test.
            [ :y => y ].
            [ :x => y ].
            [ :x => x <- a ].
            [ | x <- 5 | x ].
            y <- a.
        ).
    \}.
    ''')
