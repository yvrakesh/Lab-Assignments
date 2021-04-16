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
