module nandgate(c,a,b);
output c;
input a,b;
nand(c,a,b);
endmodule

module nandgate_3(d,c,a,b);
output d;
input a,b,c;
nand(d,c,a,b);
endmodule

module d_latch(q,q1,d,e,p,r);
output q,q1;
input d,e,p,r;
wire w1,w2;
nandgate n1(w1,d,e);
nandgate n2(w2,e,w1);
nandgate_3 n3(q,w1,q1,p);
nandgate_3 n4(q1,q,w2,r);
endmodule

module dff_f_asrl(q,q1,d,clk,r);
output q,q1;
input d,clk,r;
wire w1,clkn,wx;
nandgate n1(clkn,clk,clk);
d_latch d1(w1,wx,d,clk,1'b1,r);
d_latch d2(q,q1,w1,clkn,1'b1,r);
endmodule
