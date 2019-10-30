const xed = require('./index');

var data = new Uint8Array([0xe9, 0xfb, 0xff, 0xff, 0xff]);
var addr = "0x7fffffff00000000";
var addrLen = 16;
var instructions = xed.disassemble(data, addr, addrLen);

console.log(instructions);
