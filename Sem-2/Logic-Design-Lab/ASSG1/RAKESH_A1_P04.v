module nor_gate(c,a,b);
output c;
input a,b;
wire p,q,r;
nand(p,a,a);
nand(q,b,b);
nand(r,p,q);
nand(c,r,r);
endmodule
