module and_gate(c,a,b);

output c;
input a,b;
wire x;

nand(x,a,b);
nand(c,x,x);

endmodule

module or_gate(c,a,b);

output c;
input a,b;
wire x,y;

nand(x,a,a);
nand(y,b,b);
nand(c,x,y);

endmodule

module gated_s_r_latch(Q,Q_bar,s,r,clk);

output Q,Q_bar;
input s,r,clk;
wire x,y;

nand(x,s,clk);
nand(y,r,clk);
nand(Q,x,Q_bar);
nand(Q_bar,y,Q);

endmodule

module D_latch(Q,Q_bar,d,clk);

output Q,Q_bar;
input d,clk;
wire d_bar;

nand(d_bar,d,d);
gated_s_r_latch latch_1(Q,Q_bar,d,d_bar,clk);

endmodule

module mux_2x1(o,i0,i1,s);

output o;
input i0,i1,s;
wire x,y,s1;

nand(s1,s,s);
and_gate and1(x,i0,s1);
and_gate and2(y,i1,s);
or_gate or1(o,x,y);

endmodule

module mux_2x1_16(b,a0,a1,s);

output [15:0]b;
input [15:0]a0,a1;
input s;

mux_2x1 m[15:0](b,a0,a1,s);

endmodule

module mux_4x1_16(b,a0,a1,a2,a3,s1,s2);

output [15:0]b;
input [15:0]a0,a1,a2,a3;
input s1,s2;
wire [15:0]x0,x1;

mux_2x1_16 mux1(x0,a0,a1,s2);
mux_2x1_16 mux2(x1,a2,a3,s2);
mux_2x1_16 mux3(b,x0,x1,s1);

endmodule

module mux_8x1_16(b,a0,a1,a2,a3,a4,a5,a6,a7,s);

output [15:0]b;
input [15:0]a0,a1,a2,a3,a4,a5,a6,a7;
input [2:0]s;
wire [15:0]x0,x1;

mux_4x1_16 mux1(x0,a0,a1,a2,a3,s[1],s[0]);
mux_4x1_16 mux2(x1,a4,a5,a6,a7,s[1],s[0]);
mux_2x1_16 mux3(b,x0,x1,s[2]);

endmodule

module DFF_enable(Q,Q_bar,d,clk,en);

output Q,Q_bar;
input d,clk,en;
wire clk_bar,d_slave,d_bar,x;

nand(clk_bar,clk,clk);
mux_2x1 mux1(x,Q,d,en);
D_latch Master(d_slave,d_bar,x,clk_bar);
D_latch Slave(Q,Q_bar,d_slave,clk);

endmodule

module buffer(o,i,s);

output o;
input i,s;
wire x;

mux_2x1 mux1(o,x,i,s);

endmodule

module binary_cell_1bit(dataout,datain,clk,read,write,chipselect);

output dataout;
input datain,clk,read,write,chipselect;
wire x,y,a,b;

and_gate and1(x,read,chipselect);
and_gate and2(y,write,chipselect);
DFF_enable DFF1(a,b,datain,clk,y);
buffer buff1(dataout,a,x);

endmodule

module register_16bit(out,in,clk,read,write,chipselect);

output [15:0]out;
input [15:0]in;
input clk,read,write,chipselect;

binary_cell_1bit bin1(out[0],in[0],clk,read,write,chipselect);
binary_cell_1bit bin2(out[1],in[1],clk,read,write,chipselect);
binary_cell_1bit bin3(out[2],in[2],clk,read,write,chipselect);
binary_cell_1bit bin4(out[3],in[3],clk,read,write,chipselect);
binary_cell_1bit bin5(out[4],in[4],clk,read,write,chipselect);
binary_cell_1bit bin6(out[5],in[5],clk,read,write,chipselect);
binary_cell_1bit bin7(out[6],in[6],clk,read,write,chipselect);
binary_cell_1bit bin8(out[7],in[7],clk,read,write,chipselect);
binary_cell_1bit bin9(out[8],in[8],clk,read,write,chipselect);
binary_cell_1bit bin10(out[9],in[9],clk,read,write,chipselect);
binary_cell_1bit bin11(out[10],in[10],clk,read,write,chipselect);
binary_cell_1bit bin12(out[11],in[11],clk,read,write,chipselect);
binary_cell_1bit bin13(out[12],in[12],clk,read,write,chipselect);
binary_cell_1bit bin14(out[13],in[13],clk,read,write,chipselect);
binary_cell_1bit bin15(out[14],in[14],clk,read,write,chipselect);
binary_cell_1bit bin16(out[15],in[15],clk,read,write,chipselect);

endmodule

module demux_1x2(b0,b1,a,s);

output b0,b1;
input a,s;
wire s1;

nand(s1,s,s);
and_gate and1(b0,a,s1);
and_gate and2(b1,a,s);

endmodule

module demux_4way(a0,a1,a2,a3,b,s1,s2);

output a0,a1,a2,a3;
input b,s1,s2;
wire x0,x1;

demux_1x2 demux1(x0,x1,b,s1);
demux_1x2 demux2(a0,a1,x0,s2);
demux_1x2 demux3(a2,a3,x1,s2);

endmodule

module demux_8way(a0,a1,a2,a3,a4,a5,a6,a7,b,s1,s2,s3);

output a0,a1,a2,a3,a4,a5,a6,a7;
input b,s1,s2,s3;
wire x0,x1;

demux_1x2 demux1(x0,x1,b,s1);
demux_4way demux2(a0,a1,a2,a3,x0,s2,s3);
demux_4way demux3(a4,a5,a6,a7,x1,s2,s3);

endmodule

module decoder_3x8(out,in,en);

output [7:0]out;
input [2:0]in;
input en;

demux_8way demux(out[0],out[1],out[2],out[3],out[4],out[5],out[6],out[7],en,in[2],in[1],in[0]);

endmodule

module RAM8_16bit(out,in,clk,read,write,add,en);

output [15:0]out;
input [15:0]in;
input [2:0]add;
input clk,read,write,en;
wire [7:0]chip;
wire [15:0]i0,i1,i2,i3,i4,i5,i6,i7;

decoder_3x8 decoder(chip,add,en);
register_16bit reg1(i0,in,clk,read,write,chip[0]);
register_16bit reg2(i1,in,clk,read,write,chip[1]);
register_16bit reg3(i2,in,clk,read,write,chip[2]);
register_16bit reg4(i3,in,clk,read,write,chip[3]);
register_16bit reg5(i4,in,clk,read,write,chip[4]);
register_16bit reg6(i5,in,clk,read,write,chip[5]);
register_16bit reg7(i6,in,clk,read,write,chip[6]);
register_16bit reg8(i7,in,clk,read,write,chip[7]);
mux_8x1_16 mux1(out,i0,i1,i2,i3,i4,i5,i6,i7,add);

endmodule

