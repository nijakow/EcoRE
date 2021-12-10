# List of bytecodes

| Bytecode | Layout                               | Stack (before)      | Stack (after)  |
| -------- | ------------------------------------ | ------------------- | -------------- |
| NOOP     | `0x00`                               |                     |                |
| CONST    | `0x01 <dest:u8> <id:u16>`            |                     |                |
| PUSHC    | `0x02 <id:u16>`                      |                     | `c`            |
| PUSH     | `0x03 <src:u8>`                      |                     | `r[src]`       |
| POP      | `0x04 <dest:u8>`                     | `r[dest]`           |                |
| DROP     | `0x05`                               | `x`                 |                |
| DUP      | `0x06`                               | `x`                 | `x, x`         |
| R2R      | `0x07 <dest:u8> <src:u8>`            |                     |                |
| R2L      | `0x08 <dest:u8> <depth:u8> <src:u8>` |                     |                |
| L2R      | `0x09 <dest:u8> <src:u8> <depth:u8>` |                     |                |
| BUILTIN  | `0x0a <args:u8> <key:u16>`           | `self, a0, ..., aN` | `return_value` |
| BUILTINV | `0x0b <args:u8> <key:u16>`           | `self, a0, ..., aN` | `return_value` |
| SEND     | `0x0c <args:u8> <key:u16>`           | `self, a0, ..., aN` | `return_value` |
| SENDV    | `0x0d <args:u8> <key:u16>`           | `self, a0, ..., aN` | `return_value` |
| ASSIGN   | `0x0e <key:u16>`                     | `target, value`     | `target`       |
| AS       | `0x0f`                               | `object, type`      | `object'`      |
| RETURN   | `0x10 <depth:u8>`                    |                     |                |
| CLOSURE  | `0x11 <dest:u8> <code_id:u16>`       |                     |                |
