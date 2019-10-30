# node-xed
nodejs binding for INTEL XED (The X86 Encoder Decoder)

# usage

```javascript
const xed = require('node-xed');

var data = new Uint8Array([0xe9, 0xfb, 0xff, 0xff, 0xff]);
var addr = "0x7fffffff00000000";
var addrLen = 16;
var instructions = xed.disassemble(data, addr, addrLen);

console.log(instructions);

// result is
[ { asm: 'jmp 0x7fffffff00000000',
    addr: '0x7fffffff00000000',
    offset: 0,
    len: 5 } ]
```
