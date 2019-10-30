const xed = require('./index');

const bin = new Uint8Array([
  0xe9, 1, 0, 0, 0,
  0xe9, 1, 0, 0, 0,
  0xe9, 1, 0, 0, 0,
  0xe9, 1, 0, 0, 0,
  0xe9, 1, 0, 0, 0,
  0xe9, 1, 0, 0, 0,
])
const codes = xed.disassemble(bin, "0x8000000000000000", 20);
codes.forEach(code => {
  console.log(code.addr, code.asm, 'len=', code.len, 'offset=', code.offset);
});

