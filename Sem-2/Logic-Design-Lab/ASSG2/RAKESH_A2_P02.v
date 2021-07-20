module Full_Adder(s,cout,a,b,cin);
output s,cout;
input a,b,cin;
wire w1,w2,w3,w4,w5;
Half_Adder Half_Adder_1(w1,w2,a,b);
Half_Adder Half_Adder_2(s,w3,w1,cin);
nand(w4,w2,w2);
nand(w5,w3,w3);
nand(cout,w4,w5);
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
