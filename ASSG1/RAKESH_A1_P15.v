module mux_4_1_16(y,i1,i2,i3,i4,s1,s2);
output [15:0]y;
input [15:0]i1,i2,i3,i4,s1,s2;
mux_4_1 mux_4_16[15:0](y,i1,i2,i3,i4,s1,s2);
endmodule
module mux_4_1(y,i1,i2,i3,i4,s1,s2);
output y;
input i1,i2,i3,i4,s1,s2;
wire p,q,r,s,t,u;
nand(p,s1,s1);
nand(q,s2,s2);
nand(r,p,q,i1);
nand(s,p,s2,i2);
nand(t,s1,q,i3);
nand(u,s1,s2,i4);
nand(y,r,s,t,u);
endmodule
