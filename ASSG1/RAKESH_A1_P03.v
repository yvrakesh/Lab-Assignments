module or_gate(c,a,b);
output c;
input a,b;
wire p,q;
nand(p,a,a);
nand(q,b,b);
nand(c,p,q);
endmodule
