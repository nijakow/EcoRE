import ecosphere.parser.stream
from ecosphere.parser.tokens import TokenType as TokenType


class ParseException(Exception):

    def __init__(self, ast, message):
        super().__init__(message)
        self._ast = ast



class LabelStorage:

    def when_label_defined(self, label, callback):
        if label in self._labels:
            callback(self._labels[label])
        elif label in self._label_callbacks:
            self._label_callbacks[label] = callback
        else:
            self._label_callbacks[label] = [callback]
    
    def define_label(self, key, value):
        self._labels[key] = value
        if key in self._label_callbacks:
            callbacks = self._label_callbacks[key]
            del self._label_callbacks[key]
            for cb in callbacks:
                cb(value)

    def __init__(self):
        self._labels = dict()
        self._label_callbacks = dict()


class ParseManager:

    def when_label_defined(self, label, callback):
        self._label_storage.when_label_defined(label, callback)
    
    def define_label(self, key, value):
        self._label_storage.define_label(key, value)
    
    def parse_file(self, path):
        if path not in self._files:
            # TODO: Insert sentinel value to avoid endless recursions
            with open(path, 'r') as f:
                text_stream = ecosphere.parser.stream.TextStream(f.read())
                the_parser = text_stream.get_parser(self)
                self._files[path] = the_parser.parse()
        return self._files[path]

    def __init__(self):
        self._label_storage = LabelStorage()
        self._files = dict()


class Parser:

    def check(self, token_type):
        t = self._t.read()
        if t.isa(token_type):
            return t
        else:
            t.fail()
            return False
    
    def check_identifier(self):
        return self.check(TokenType.IDENTIFIER)
    
    def check_key(self):
        return self.check(TokenType.KEY)
    
    def expect(self, token_type):
        t = self._t.read()
        if t.isa(token_type):
            return t
        else:
            t.fail()
            raise ParseException(t, 'Expected different token type: ' + str(token_type) + ', got: ' + str(t))
    
    def expect_identifier(self):
        return self.expect(TokenType.IDENTIFIER)

    def get_tokenizer(self):
        return self._t
    
    def get_pm(self):
        return self._pm

    def __init__(self, tokenizer, parse_manager=None):
        self._t = tokenizer
        self._pm = parse_manager or ParseManager()


class SubParser(Parser):

    def get_parent(self):
        return self._parent

    def __init__(self, parent):
        super().__init__(parent.get_tokenizer(), parent.get_pm())
        self._parent = parent
