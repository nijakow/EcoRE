#!/usr/bin/python3
import sys

def convert(infile):
    print('#define ECO_CONFIG_COMPILE_WITH_INTEGRATED_BINARY')
    print()
    print('char Eco_DEFAULT_IMAGE[] = ')
    print('{')
    print('    ', end='')
    i = 0
    while True:
        bts = infile.read(1024)
        if not bts:
            break        
        for b in bts:
            print('0x{:02x}, '.format(b), end='')
            i += 1
            if i % 8 == 0:
                print()
                print('    ', end='')
    if i % 8 != 0:
        print()
    print('};')
    print()
    print('const unsigned int Eco_DEFAULT_IMAGE_LENGTH =', i, ';')


def convert_files(inpath, outpath):
    with open(inpath, 'rb') as infile, open(outpath, 'w') as outfile:
        sys.stdout = outfile
        convert(infile)

if __name__ == '__main__':
    convert_files(sys.argv[1], sys.argv[2])
