import enum

import datatypes


class Bytecodes(enum.IntEnum):
    NOOP = 0x00
    CONST2R = 0x01
    PUSHC = 0x02
    PUSHR = 0x03
    POPR = 0x04
    DROP = 0x05
    DUP = 0x06
    R2R = 0x07
    R2L = 0x08
    L2R = 0x09
    BUILTIN = 0x0a
    SEND = 0x0b
    ASSIGN = 0x0c
    RET = 0x0d
    MAKE_CLOSURE = 0x0e


class CodeGenerator:

    def _add_u8(self, u8):
        self._instructions.append(u8 & 0xff)

    def _add_u16(self, u16):
        self._add_u8(u16 >> 8)
        self._add_u8(u16)

    def _add_constant(self, value):
        constant_id = len(self._constants)
        self._constants.append(value)
        self._add_u16(constant_id)
        return constant_id

    def add_noop(self):
        self._add_u8(Bytecodes.NOOP)

    def add_const2r(self, r, value):
        self._add_u8(Bytecodes.CONST2R)
        self._add_u8(r)
        self._add_constant(value)
    
    def add_pushc(self, value):
        self._add_u8(Bytecodes.PUSHC)
        self._add_constant(value)

    def add_pushr(self, r):
        self._add_u8(Bytecodes.PUSHR)
        self._add_u8(r)

    def add_popr(self, r):
        self._add_u8(Bytecodes.POPR)
        self._add_u8(r)

    def add_dup(self):
        self._add_u8(Bytecodes.DUP)

    def add_drop(self):
        self._add_u8(Bytecodes.DROP)

    def add_r2r(self, dest, src):
        self._add_u8(Bytecodes.R2R)
        self._add_u8(dest)
        self._add_u8(src)

    def add_r2l(self, dest, depth, src):
        self._add_u8(Bytecodes.R2L)
        self._add_u8(dest)
        self._add_u8(depth)
        self._add_u8(src)

    def add_l2r(self, dest, src, depth):
        self._add_u8(Bytecodes.L2R)
        self._add_u8(dest)
        self._add_u8(src)
        self._add_u8(depth)

    def add_builtin(self, arg_count, key):
        self._add_u8(Bytecodes.BUILTIN)
        self._add_u8(arg_count)
        self._add_constant(key)

    def add_send(self, arg_count, key):
        self._add_u8(Bytecodes.SEND)
        self._add_u8(arg_count)
        self._add_constant(key)

    def add_assign(self, key):
        self._add_u8(Bytecodes.ASSIGN)
        self._add_constant(key)

    def add_return(self, depth):
        self._add_u8(Bytecodes.RET)
        self._add_u8(depth)

    def add_make_closure(self, dest, code):
        self._add_u8(Bytecodes.MAKE_CLOSURE)
        self._add_u8(dest)
        self._add_u8(len(self._subcodes))
        self._subcodes.append(code)
    
    def finish(self, local_count, param_count):
        return datatypes.Code(local_count,
                              param_count,
                              self._constants,
                              self._subcodes,
                              self._instructions)
    
    def __init__(self):
        self._instructions = bytearray()
        self._constants = []
        self._subcodes = []
