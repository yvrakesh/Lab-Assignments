module Negator_16(b,a);
output [15:0]b;
input [15:0]a;
not_gate not_gate_1 [15:0](b,a);
endmodule
module not_gate(b,a);
output b;
input a;
nand(b,a,a);
endmodule
