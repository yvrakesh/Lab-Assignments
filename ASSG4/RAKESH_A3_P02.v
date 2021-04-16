module onebitbinarycell(d_out,d_in,w,r,cs,clk);
output d_out;
input d_in,w,r,cs,clk;
wire w1,w2,w3;
and_gate and1(w1,w,cs);
mux_2_2 m1(w2,q,d_in,w1);
dflipflop_rise d1(q,q_bar,clk,w2);
and_gate and2(w3,r,cs);
mux_2_2 m2(d_out,1'bz,q,w3);
endmodule

module and_gate(c,a,b);
output c;
input a,b;
wire w1;
nand(w1,a,b);
nand(c,w1,w1);
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

module or_gate(c,a,b);
output c;
input a,b;
wire w1,w2;
nand(w1,a,a);
nand(w2,b,b);
nand(c,w1,w2);
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
nand(w2,clk,d);
nand(w3,clk,w1);
nand(q,q_bar,w2);
nand(q_bar,q,w3);
endmodule
