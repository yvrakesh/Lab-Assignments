module multiplex_16(y,s,a,b);
output [15:0]y;
input [15:0]s,a,b;
mux_1_2 mux2[15:0](y,s,a,b);
endmodule

module mux_1_2(y,s,a,b);
output y;
input s,a,b;
wire p,q,r;
nand(p,s,s);
nand(q,p,a);
nand(r,s,b);
nand(y,q,r);
endmodule