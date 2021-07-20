module or_gate_16(c,a,b);
output [15:0]c;
input [15:0]a,b;
or_gate or_gate2[15:0](c,a,b);
endmodule

module or_gate(c,a,b);
output c;
input a,b;
wire p,q;
nand(p,a,a);
nand(q,b,b);
nand(c,p,q);
endmodule