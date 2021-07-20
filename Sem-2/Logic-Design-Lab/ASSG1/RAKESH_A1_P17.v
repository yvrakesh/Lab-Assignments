module demux_1_4(o1,o2,o3,o4,i1,s1,s2);
output o1,o2,o3,o4;
input i1,s1,s2;
nand(p,s1,s1);
nand(q,s2,s2);
nand(r,p,q,i1);
nand(o1,r,r);
nand(s,p,s2,i1);
nand(o2,s,s);
nand(t,s1,q,i1);
nand(o3,t,t);
nand(u,s1,s2,i1);
nand(o4,u,u);
endmodule
