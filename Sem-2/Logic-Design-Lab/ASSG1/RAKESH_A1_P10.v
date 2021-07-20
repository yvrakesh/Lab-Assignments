module xor_gate_16(c,a,b);
output [15:0]c;
input [15:0]a,b;
xor_gate xor_gate2[15:0](c,a,b);
endmodule

module xor_gate(c,a,b);
output c;
input a,b;
wire p,q,r,s;
nand(p,a,a);
nand(q,b,b);
nand(r,a,q);
nand(s,b,p);
nand(c,r,s);
endmodule