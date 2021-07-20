module onebitbinarycell(d_out,d_in,cs,w,r,clk);
output d_out;
input d_in,cs,w,r,clk;
and_gate and1(w1,w,cs);
and_gate and2(w2,r,cs);
mux_2_1 mux1(w3,q,d_in,w1);
dflipflop_rise dflipflop(q,q_bar,w3,clk);
mux_2_1 mux2(d_out,1'bz,q,w2);
endmodule

module mux_2_1(c,a,b,s);
output c;
input a,b,s;
wire w1,w2,w3;
nand(w1,s,s);
and_gate and1(w2,a,w1);
and_gate and2(w3,b,s);
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

module dflipflop_rise(q,q_bar,d,clk);
output q,q_bar;
input d,clk;
wire w1,w2,w3;
nand(w1,clk,clk);
dlatch dlatch1(w2,w3,d,w1);
dlatch dlatch2(q,q_bar,w2,clk);
endmodule

module dlatch(q,q_bar,d,clk);
output q,q_bar;
input d,clk;
wire w1,w2,w3;
nand(w1,d,d);
nand(w2,d,clk);
nand(w3,w1,clk);
nand(q,w2,q_bar);
nand(q_bar,w3,q);
endmodule
