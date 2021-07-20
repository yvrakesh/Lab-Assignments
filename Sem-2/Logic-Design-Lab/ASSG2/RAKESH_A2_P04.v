module Adder_4(c,carry,a,b);
output [3:0]c;
output carry;
input [3:0]a,b;
wire w1,w2,w3;
Half_Adder Half_Adder_1(c[0],w1,a[0],b[0]);
Full_Adder Fulle_Adder_1(c[1],w2,a[1],b[1],w1);
Full_Adder Full_Adder_2(c[2],w3,a[2],b[2],w2);
Full_Adder Full_Adder_3(c[3],carry,a[3],b[3],w3);
endmodule


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


