#!/usr/bin/python3

import sys
import pathlib

import ecosphere.parser.core
import ecosphere.econnect


class Bootstrapper:

    def construct_from_file(self, path):
        path = pathlib.Path(path)
        self._lobby = self._parse_manager.load_file(path)

    def serialize(self):
        serializer = ecosphere.econnect.Serializer()
        self._lobby.compile_as_code().serialize(serializer)
        return serializer.finish()

    def write_to_stream(self, stream):
        stream.write(self.serialize())

    def write_to_file(self, path):
        with open(path, 'wb') as f:
            self.write_to_stream(f)

    def __init__(self):
        self._lobby = None
        self._parse_manager = ecosphere.parser.core.ParseManagerCore()


def bootstrap():
    strapper = Bootstrapper()
    if len(sys.argv) != 3:
        raise Exception('Usage: ' + sys.argv[0] + ' <source> <target.ebf>')
    print('Bootstrapping from', sys.argv[1])
    strapper.construct_from_file(sys.argv[1])
    strapper.write_to_file(sys.argv[2])


if __name__ == '__main__':
    bootstrap()
