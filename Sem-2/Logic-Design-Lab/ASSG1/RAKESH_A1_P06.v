module xnor_gate(c,a,b);
output c;
input a,b;
wire p,q,r,s;
nand(p,a,a);
nand(q,b,b);
nand(r,a,b);
nand(s,p,q);
nand(c,r,s);
endmodule
