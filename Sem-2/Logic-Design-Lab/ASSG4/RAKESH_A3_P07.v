
module ram4k(d_out,d_in,w,r,en,clk,add);
output [15:0]d_out;
input [15:0]d_in;
input w,r,en,clk;
input [11:0]add;
wire [7:0]w1;
wire [15:0]i0,i1,i2,i3,i4,i5,i6,i7;
decoder_3_8 dec1(w1,add[11:9],en);
ram512 r1(i0,d_in,w,r,w1[0],clk,add[8:0]);
ram512 r2(i1,d_in,w,r,w1[1],clk,add[8:0]);
ram512 r3(i2,d_in,w,r,w1[2],clk,add[8:0]);
ram512 r4(i3,d_in,w,r,w1[3],clk,add[8:0]);
ram512 r5(i4,d_in,w,r,w1[4],clk,add[8:0]);
ram512 r6(i5,d_in,w,r,w1[5],clk,add[8:0]);
ram512 r7(i6,d_in,w,r,w1[6],clk,add[8:0]);
ram512 r8(i7,d_in,w,r,w1[7],clk,add[8:0]);
mux_8_3_16 mux1(d_out,add[11:9],i0,i1,i2,i3,i4,i5,i6,i7);
endmodule

module ram512(d_out,d_in,w,r,en,clk,add);
output [15:0]d_out;
input [15:0]d_in;
input w,r,en,clk;
input [8:0]add;
wire [7:0]w1;
wire [15:0]i0,i1,i2,i3,i4,i5,i6,i7;
decoder_3_8 dec1(w1,add[8:6],en);
ram64 r1(i0,d_in,w,r,w1[0],clk,add[5:0]);
ram64 r2(i1,d_in,w,r,w1[1],clk,add[5:0]);
ram64 r3(i2,d_in,w,r,w1[2],clk,add[5:0]);
ram64 r4(i3,d_in,w,r,w1[3],clk,add[5:0]);
ram64 r5(i4,d_in,w,r,w1[4],clk,add[5:0]);
ram64 r6(i5,d_in,w,r,w1[5],clk,add[5:0]);
ram64 r7(i6,d_in,w,r,w1[6],clk,add[5:0]);
ram64 r8(i7,d_in,w,r,w1[7],clk,add[5:0]);
mux_8_3_16 mux1(d_out,add[8:6],i0,i1,i2,i3,i4,i5,i6,i7);
endmodule

module ram64(d_out,d_in,w,r,en,clk,add);
output [15:0]d_out;
input [15:0]d_in;
input w,r,en,clk;
input [5:0]add;
wire [7:0]w1;
wire [15:0]i0,i1,i2,i3,i4,i5,i6,i7;
decoder_3_8 dec1(w1,add[5:3],en);
ram8 r1(i0,d_in,w,r,w1[0],clk,add[2:0]);
ram8 r2(i1,d_in,w,r,w1[1],clk,add[2:0]);
ram8 r3(i2,d_in,w,r,w1[2],clk,add[2:0]);
ram8 r4(i3,d_in,w,r,w1[3],clk,add[2:0]);
ram8 r5(i4,d_in,w,r,w1[4],clk,add[2:0]);
ram8 r6(i5,d_in,w,r,w1[5],clk,add[2:0]);
ram8 r7(i6,d_in,w,r,w1[6],clk,add[2:0]);
ram8 r8(i7,d_in,w,r,w1[7],clk,add[2:0]);
mux_8_3_16 mux1(d_out,add[5:3],i0,i1,i2,i3,i4,i5,i6,i7);
endmodule 

module ram8(d_out,d_in,w,r,en,clk,add);
output [15:0]d_out;
input [15:0]d_in;
input w,r,en,clk;
input [2:0]add;
wire [7:0]w1;
wire [15:0]i0,i1,i2,i3,i4,i5,i6,i7,i8;
decoder_3_8 dec1(w1,add,en);
sixteenbitregister r1(i0,d_in,w,r,w1[0],clk);
sixteenbitregister r2(i1,d_in,w,r,w1[1],clk);
sixteenbitregister r3(i2,d_in,w,r,w1[2],clk);
sixteenbitregister r4(i3,d_in,w,r,w1[3],clk);
sixteenbitregister r5(i4,d_in,w,r,w1[4],clk);
sixteenbitregister r6(i5,d_in,w,r,w1[5],clk);
sixteenbitregister r7(i6,d_in,w,r,w1[6],clk);
sixteenbitregister r8(i7,d_in,w,r,w1[7],clk);
mux_8_3_16 mux1(d_out,add,i0,i1,i2,i3,i4,i5,i6,i7);
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

module decoder_3_8(c,s,a);
output [7:0]c;
input [2:0]s;
input a;
wire w1,w2;
demux_2_1 dem1(w1,w2,s[0],a);
demux_4_2 dem2(c[0],c[2],c[4],c[6],s[2],s[1],w1);
demux_4_2 dem3(c[1],c[3],c[5],c[7],s[2],s[1],w2);
endmodule

module demux_4_2(a0,a1,a2,a3,s1,s0,a);
output a0,a1,a2,a3;
input s1,s0,a;
demux_2_1 dem1(w1,w2,s0,a);
demux_2_1 dem2(a0,a2,s1,w1);
demux_2_1 dem3(a1,a3,s1,w2);
endmodule

module demux_2_1(a0,a1,s0,a);
output a0,a1;
input s0,a;
wire w1;
nand(w1,s0,s0);
and_gate and1(a0,w1,a);
and_gate and2(a1,s0,a);
endmodule

module sixteenbitregister(d_out,d_in,w,r,cs,clk);
output [15:0]d_out;
input [15:0]d_in;
input w,r,cs,clk;
onebitbinarycell one1[15:0](d_out,d_in,w,r,cs,clk);
endmodule

module onebitbinarycell(d_out,d_in,w,r,cs,clk);
output d_out;
input d_in,w,r,cs,clk;
wire w1,w2,w3;
and_gate and1(w1,w,cs);
mux_2_1 mux1(w2,w1,q,d_in);
dflipflop_rise d1(q,q_bar,clk,w2);
and_gate and2(w3,r,cs);
mux_2_1 mux2(d_out,w3,1'bz,q);
endmodule

module dflipflop_rise(q,q_bar,clk,d);
output q,q_bar;
input clk,d;
wire w1,q1,q1_bar;
nand(w1,clk,clk);
dlatch d1(q1,q1_bar,w1,d);
dlatch d2(q,q_bar,clk,q1);
endmodule

module dlatch(q,q_bar,clk,d);
output q,q_bar;
input clk,d;
wire w1,w2,w3;
nand(w1,d,d);
nand(w2,w1,clk);
nand(w3,d,clk);
nand(q,q_bar,w2);
nand(q_bar,q,w3);
endmodule
