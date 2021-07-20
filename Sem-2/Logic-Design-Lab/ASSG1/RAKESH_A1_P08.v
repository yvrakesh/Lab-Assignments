module and_gate_16(c,a,b);
output [15:0]c;
input [15:0]a;
input [15:0]b;
and_gate and_gate_2[15:0](c,a,b);
endmodule

module and_gate(c,a,b);
output c;
input a,b;
wire x;
nand(x,a,b);
nand(c,x,x);
endmodule