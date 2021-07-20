
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
