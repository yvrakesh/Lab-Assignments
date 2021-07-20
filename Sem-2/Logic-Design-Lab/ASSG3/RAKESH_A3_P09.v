
module nandgate(c,a,b);
output c;
input a,b;
nand(c,a,b);
endmodule
module d_latch(q,q1,d,e);
output q,q1;
input d,e;
wire w1,w2;
nandgate n1(w1,d,e);
nandgate n2(w2,e,w1);
nandgate n3(q,w1,q1);
nandgate n4(q1,q,w2);
endmodule
module mux(c,s,a,b);
output c;
input s,a,b;
wire x,y,z;
notgate not1(x,s);
andgate and1(y,x,a);
andgate and2(z,s,b);
orgate or1(c,y,z);
endmodule
module notgate(b,a);
output b;
input a;
nand(b,a,a);
endmodule
module andgate(c,a,b);
output c;
input a,b;
wire x;
nand(x,a,b);
notgate notgate_1(c,x);
endmodule
module orgate(c,a,b);
output c;
input a,b;
wire x,y;
nand(x,a,a);
nand(y,b,b);
nand(c,x,y);
endmodule
module d_flipflop_r(q,q1,d,clk);
output q,q1;
input d,clk;
wire w1,clkn,wx;
nandgate n1(clkn,clk,clk);
d_latch d1(w1,wx,d,clkn);
d_latch d2(q,q1,w1,clk);
endmodule
module bin_cell(d_o,d_i,cs,rd,wr,clk);
output d_o;
input d_i,cs,rd,wr,clk;
wire q,q1,d,en,enr;
andgate a1(en,wr,cs);
mux m1(d,en,q,d_i);
d_flipflop_r d1(q,q1,d,clk);
andgate a2(enr,cs,rd);
mux m2(d_o,enr,1'bz,q);
endmodule
module reg_16(d_o,d_i,cs,r,w,clk);
output [15:0] d_o;
input [15:0] d_i;
input r,w,cs,clk;
bin_cell bc1[15:0](d_o,d_i,cs,r,w,clk);
endmodule
module xorgate(c,a,b);
output c;
input a,b;
wire x,y,a1,b1;
notgate not1(a1,a);
notgate not2(b1,b);
andgate and1(x,a,b1);
andgate and2(y,a1,b);
orgate or1(c,x,y);
endmodule
module halfadder(c,s,a,b);
output s,c;
input a,b;
xorgate x1(s,a,b);
andgate and1(c,a,b);
endmodule
module fulladder(cout,s,cin,a,b);
output cout,s;
input cin,a,b;
wire c1,s1,c2;
halfadder h1(c1,s1,a,b);
halfadder h2(c2,s,s1,cin);
orgate or1(cout,c1,c2);
endmodule
module fulladder_4(cout,s,cin,a,b);
output [3:0]s;
output cout;
input [3:0]a,b;
input cin;
wire c1,c2,c3;
fulladder f1(c1,s[0],cin,a[0],b[0]);
fulladder f2(c2,s[1],c1,a[1],b[1]);
fulladder f3(c3,s[2],c2,a[2],b[2]);
fulladder f4(cout,s[3],c3,a[3],b[3]);
endmodule
module fulladder_16(cout,s,cin,a,b);
output [15:0]s;
output cout;
input [15:0]a,b;
input cin;
wire c1,c2,c3;
fulladder_4 f1(c1,s[3:0],cin,a[3:0],b[3:0]);
fulladder_4 f2(c2,s[7:4],c1,a[7:4],b[7:4]);
fulladder_4 f3(c3,s[11:8],c2,a[11:8],b[11:8]);
fulladder_4 f4(cout,s[15:12],c3,a[15:12],b[15:12]);
endmodule
module mux16(c,s,a,b);
output [15:0] c;
input [15:0] a,b;
input s;
mux mux1[15:0](c,s,a,b);
endmodule

module pc(out,in,inc,load,reset,clk);
output [15:0] out;
input [15:0] in;
input reset,inc,load,clk;
wire [15:0]w1,w2t,w2;
fulladder_16 f1(,w1,inc,out,16'b0000000000000000);
mux16 mux161(w2t,load,w1,in);
mux16 mux162(w2,reset,w2t,16'b0000000000000000);
reg_16 r11(out,w2,1'b1,1'b1,1'b1,clk);
endmodule