
module pc(d_out,d_in,inc,load,reset,clk);
output [15:0] d_out;
input [15:0] d_in;
input reset,inc,load,clk;
wire [15:0]w1,w2,w3;
Adder_16 add(w1,inc,d_out,16'b0000000000000000);
mux_2_1_16 mux1(w2,load,w1,16'b0000000000000000);
mux_2_1_16 mux2(w3,reset,w2,16'b0000000000000000);
sixteenbitregister s1(d_out,w3,1'b1,1'b1,1'b1,clk);
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

module mux_8_3_16(c,s,a0,a1,a2,a3,a4,a5,a6,a7);
output [15:0]c;
input [2:0]s;
input [15:0]a0,a1,a2,a3,a4,a5,a6,a7;
wire [15:0]w1,w2;
mux_4_2_16 mux1(w1,s[2],s[1],a0,a2,a4,a6);
mux_4_2_16 mux2(w2,s[2],s[1],a1,a3,a5,a7);
mux_2_1_16 mux3(c,s[0],w1,w2);
endmodule

module mux_4_2_16(c,s1,s0,a0,a1,a2,a3);
output [15:0]c;
input s1,s0;
input [15:0] a0,a1,a2,a3;
wire [15:0]w1,w2;
mux_2_1_16 mux1(w1,s1,a0,a2);
mux_2_1_16 mux2(w2,s1,a1,a3);
mux_2_1_16 mux3(c,s0,w1,w2);
endmodule

module mux_2_1_16(c,s,a,b);
output [15:0]c;
input s;
input [15:0]a,b;
mux_2_1 mux1[15:0](c,s,a,b);
endmodule

module mux_2_1(c,s,a,b);
output c;
input s,a,b;
wire w1,w2,w3;
nand(w1,s,s);
and_gate and1(w2,w1,a);
and_gate and2(w3,s,b);
or_gate or1(c,w2,w3);
endmodule

module and_gate(c,a,b);
output c;
input a,b;
wire w1;
nand(w1,a,b);
nand(c,w1,w1);
endmodule

module or_gate(c,a,b);
output c;
input a,b;
wire w1,w2;
nand(w1,a,a);
nand(w2,b,b);
nand(c,w1,w2);
endmodule

module sixteenbitregister(d_out,d_in,w,r,cs,clk);
output [15:0]d_out;
input [15:0]d_in;
input w,r,cs,clk;
onebitbinarycell ob1[15:0](d_out,d_in,w,r,cs,clk);
endmodule

module onebitbinarycell(d_out,d_in,w,r,cs,clk);
output d_out;
input d_in,w,r,cs,clk;
wire w1,w2,w3;
and_gate and1(w1,w,cs);
mux_2_2 mux1(w2,q,d_in,w1);
dflipflop_rise d1(q,q_bar,clk,w2);
and_gate and2(w3,r,cs);
mux_2_2 mux2(d_out,1'bz,q,w3);
endmodule


module mux_2_2(c,a,b,s);
output c;
input a,b,s;
wire w1,w2,w3;
nand(w1,s,s);
and_gate and1(w2,w1,a);
and_gate and2(w3,s,b);
or_gate or1(c,w2,w3);
endmodule

module dflipflop_rise(q,q_bar,clk,d);
output q,q_bar;
input clk,d;
wire w1;
nand(w1,clk,clk);
dlatch d1(q1,q1_bar,w1,d);
dlatch d2(q,q_bar,clk,q1);
endmodule

module dlatch(q,q_bar,clk,d);
output q,q_bar;
input clk,d;
wire w1,w2,w3;
nand(w1,d,d);
nand(w2,clk,d);
nand(w3,clk,w1);
nand(q,q_bar,w2);
nand(q_bar,q,w3);
endmodule
