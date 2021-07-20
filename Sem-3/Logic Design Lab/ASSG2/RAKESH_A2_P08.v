module alu(c,zr,ng,a,b,s);
output [15:0]c;
output zr,ng;
input [15:0]a,b;
input [5:0]s;
wire [15:0]p,q,r,t,u,v,w,x,y,z;
wire carry,a1,a2,a3,a4;
mux_2_1 mux_1(p,a,16'b0,s[5]);
mux_2_1 mux_2(q,b,16'b0,s[3]);
not_gate_16 not_gate_1(r,p);
mux_2_1 mux_3(t,p,r,s[4]);
not_gate_16 not_gate_2(u,q);
mux_2_1 mux_4(v,q,u,s[2]);
Adder_16 Adder_1(w,carry,t,v);
and_gate_16 and_gate_3(x,t,v);
mux_2_1 mux_5(y,x,w,s[1]);
not_gate_16 not_gate_3(z,y);
mux_2_1 mux_6(c,y,z,s[0]);
or_gate or_gate_1(a1,c[15],c[14]);
or_gate or_gate_2(a2,c[13],c[12]);
or_gate or_gate_3(a3,c[11],c[10]);
or_gate or_gate_4(a4,c[9],c[8]);
or_gate or_gate_5(a5,c[7],c[6]);
or_gate or_gate_6(a6,c[5],c[4]);
or_gate or_gate_7(a7,c[3],c[2]);
or_gate or_gate_8(a8,c[1],c[0]);
or_gate or_gate_9(a9,a1,a2);
or_gate or_gate_10(a10,a3,a4);
or_gate or_gate_11(a11,a5,a6);
or_gate or_gate_12(a12,a7,a8);
or_gate or_gate_13(a13,a9,a10);
or_gate or_gate_14(a14,a11,a12);
nor_gate nor_gate_15(zr,a13,a14);
and_gate and_gate_5(ng,1'b1,c[15]);
endmodule

module mux_2_1(c,a,b,s);
output [15:0]c;
input [15:0]a,b;
input s;
mux_2_1_1 mux_1[15:0](c,a,b,s);
endmodule

module mux_2_1_1(c,a,b,s);
output c;
input a,b,s;
wire p,r;
wire q;
nand(q,s,s);
and_gate and_gate_1(p,q,a);
and_gate and_gate_2(r,s,b);
or_gate or_gate_1(c,p,r);
endmodule

module not_gate(b,a);
output b;
input a;
nand(b,a,a);
endmodule

module and_gate(c,a,b);
output c;
input a,b;
wire p;
nand(p,a,b);
nand(c,p,p);
endmodule

module or_gate(c,a,b);
output c;
input a,b;
wire p,q;
nand(p,a,a);
nand(q,b,b);
nand(c,p,q);
endmodule

module not_gate_16(b,a);
output [15:0]b;
input [15:0]a;
not_gate not_gate_1[15:0](b,a);
endmodule

module and_gate_16(c,a,b);
output [15:0]c;
input [15:0]a,b;
and_gate and_gate_1[15:0](c,a,b);
endmodule

module nor_gate_16(c,a,b);
output [15:0]c;
input [15:0]a,b;
nor_gate nor_gate_1[15:0](c,a,b);
endmodule

module Full_Adder(s,cout,a,b,cin);
output s,cout;
input a,b,cin;
wire w1,w2,w3,w4,w5;
Half_Adder Half_Adder_1(w1,w2,a,b);
Half_Adder Half_Adder_2(s,w3,w1,cin);
nand(w4,w2,w2);
nand(w5,w3,w3);
nand(cout,w4,w5);
endmodule

module Half_Adder(s,c,a,b);
output s,c;
input a,b;
wire w1,w2,w3,w4,w5;
nand(w1,a,a);
nand(w2,b,b);
nand(w3,a,w2);
nand(w4,b,w1);
nand(s,w3,w4);
nand(w5,a,b);
nand(c,w5,w5);
endmodule

module Adder_16(c,carry,a,b);
output [15:0]c;
output carry;
input [15:0]a,b;
wire w1,w2,w3,w4,w5,w6,w7,w8,w9,w10,w11,w12,w13,w14,w15;
Half_Adder Half_Adder_1(c[0],w1,a[0],b[0]);
Full_Adder Full_Adder_1(c[1],w2,a[1],b[1],w1);
Full_Adder Full_Adder_2(c[2],w3,a[2],b[2],w2);
Full_Adder Full_Adder_3(c[3],w4,a[3],b[3],w3);
Full_Adder Full_Adder_4(c[4],w5,a[4],b[4],w4);
Full_Adder Full_Adder_5(c[5],w6,a[5],b[5],w5);
Full_Adder Full_Adder_6(c[6],w7,a[6],b[6],w6);
Full_Adder Full_Adder_7(c[7],w8,a[7],b[7],w7);
Full_Adder Full_Adder_8(c[8],w9,a[8],b[8],w8);
Full_Adder Full_Adder_9(c[9],w10,a[9],b[9],w9);
Full_Adder Full_Adder_10(c[10],w11,a[10],b[10],w10);
Full_Adder Full_Adder_11(c[11],w12,a[11],b[11],w11);
Full_Adder Full_Adder_12(c[12],w13,a[12],b[12],w12);
Full_Adder Full_Adder_13(c[13],w14,a[13],b[13],w13);
Full_Adder Full_Adder_14(c[14],w15,a[14],b[14],w14);
Full_Adder Full_Adder_15(c[15],carry,a[15],b[15],w15);
endmodule