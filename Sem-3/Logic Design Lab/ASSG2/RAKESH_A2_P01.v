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
