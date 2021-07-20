module Adder_16(c,carry,a,b);
output [15:0]c;
output carry;
input [15:0]a,b;
wire w1,w2,w3,w4,w5,w6,w7,w8,w9,w10,w11,w12,w13,w14,w15;
Half_Adder Half_Adder_1(c[0],w1,a[0],b[0]);
Full_Adder Full_Adder_1(c[1],w2,a[1],b[1],w1);
Full_Adder Full_Adder_2(c[2],w3,a[2],b[2],w2);
Full_Adder Full_Adder_3(c[3],w4,a[3],b[3],w3);
Full_Adder Full_Adder_4(c[4],w5,a[4],b[4],w4);
Full_Adder Full_Adder_5(c[5],w6,a[5],b[5],w5);
Full_Adder Full_Adder_6(c[6],w7,a[6],b[6],w6);
Full_Adder Full_Adder_7(c[7],w8,a[7],b[7],w7);
Full_Adder Full_Adder_8(c[8],w9,a[8],b[8],w8);
Full_Adder Full_Adder_9(c[9],w10,a[9],b[9],w9);
Full_Adder Full_Adder_10(c[10],w11,a[10],b[10],w10);
Full_Adder Full_Adder_11(c[11],w12,a[11],b[11],w11);
Full_Adder Full_Adder_12(c[12],w13,a[12],b[12],w12);
Full_Adder Full_Adder_13(c[13],w14,a[13],b[13],w13);
Full_Adder Full_Adder_14(c[14],w15,a[14],b[14],w14);
Full_Adder Full_Adder_15(c[15],carry,a[15],b[15],w15);
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
