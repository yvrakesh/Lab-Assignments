module dflipflop_fall_syn(q,q_bar,d,clk,reset);
output q,q_bar;
input d,clk,reset;
wire w1,w2;
nand(w1,reset,reset);
and_gate and1(w2,d,w1);
dflipflop_fall dflipflop2(q,q_bar,w2,clk);
endmodule

module and_gate(c,a,b);
output c;
input a,b;
wire w1;
nand(w1,a,b);
nand(c,w1,w1);
endmodule

module dflipflop_fall(q,q_bar,d,clk);
output q,q_bar;
input d,clk;
wire w1,w2,w3;
nand(w1,clk,clk);
dlatch dlatch1(w2,w3,d,clk);
dlatch dlatch2(q,q_bar,w2,w1);
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
