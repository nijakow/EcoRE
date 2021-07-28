# List of bytecodes

| Bytecode | Layout                               | Stack (before)      | Stack (after)  |
| -------- | ------------------------------------ | ------------------- | -------------- |
| NOOP     | `0x00`                               |                     |                |
| CONST    | `0x01 <dest:u8> <id:u16>`            |                     |                |
| PUSH     | `0x02 <src:u8>`                      |                     | `r[src]`       |
| POP      | `0x03 <dest:u8>`                     | `r[dest]`           |                |
| DROP     | `0x04`                               | `x`                 |                |
| DUP      | `0x05`                               | `x`                 | `x, x`         |
| R2R      | `0x06 <dest:u8> <src:u8>`            |                     |                |
| R2L      | `0x07 <dest:u8> <depth:u8> <src:u8>` |                     |                |
| L2R      | `0x08 <dest:u8> <src:u8> <depth:u8>` |                     |                |
| BUILTIN  | `0x09 <args:u8> <key:u16>`           | `self, a0, ..., aN` | `return_value` |
| SEND     | `0x0a <args:u8> <key:u16>`           | `self, a0, ..., aN` | `return_value` |
| ASSIGN   | `0x0b <key:u16>`                     | `target, value`     | `target`       |
| RETURN   | `0x0c <depth:u8>`                    |                     |                |
| CLOSURE  | `0x0d <dest:u8> <code_id:u8>`        |                     |                |
