module Incrementor_4(b,a);
output [3:0]b;
input [3:0]a;
Half_Adder Half_Addder_1(b[0],w1,a[0],1);
Half_Adder Half_Adder_2(b[1],w2,a[1],w1);
Half_Adder Half_Adder_3(b[2],w3,a[2],w2);
Half_Adder Half_Adder_4(b[3],w4,a[3],w3);
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