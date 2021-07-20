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
