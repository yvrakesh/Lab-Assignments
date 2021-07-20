module Incrementor_16(b,carry,a);
output [15:0]b;
output carry;
input [15:0]a;
wire w1,w2,w3,w4,w5,w6,w7,w8,w9,w10,w11,w12,w13,w14,w15;
Half_Adder Half_Adder_1(b[0],w1,a[0],1'b1);
Half_Adder Half_Adder_2(b[1],w2,a[1],w1);
Half_Adder Half_Adder_3(b[2],w3,a[2],w2);
Half_Adder Half_Adder_4(b[3],w4,a[3],w3);
Half_Adder Half_Adder_5(b[4],w5,a[4],w4);
Half_Adder Half_Adder_6(b[5],w6,a[5],w5);
Half_Adder Half_Adder_7(b[6],w7,a[6],w6);
Half_Adder Half_Adder_8(b[7],w8,a[7],w7);
Half_Adder Half_Adder_9(b[8],w9,a[8],w8);
Half_Adder Half_Adder_10(b[9],w10,a[9],w9);
Half_Adder Half_Adder_11(b[10],w11,a[10],w10);
Half_Adder Half_Adder_12(b[11],w12,a[11],w11);
Half_Adder Half_Adder_13(b[12],w13,a[12],w12);
Half_Adder Half_Adder_14(b[13],w14,a[13],w13);
Half_Adder Half_Adder_15(b[14],w15,a[14],w14);
Half_Adder Half_Adder_16(b[15],carry,a[15],w15);
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