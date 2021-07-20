module demux_1_2(o1,o2,i,s);
output o1,o2;
input i,s;
wire p,q,r;
nand(p,s,s);
nand(q,p,i);
nand(o1,q,q);
nand(r,i,s);
nand(o2,r,r);
endmodule
