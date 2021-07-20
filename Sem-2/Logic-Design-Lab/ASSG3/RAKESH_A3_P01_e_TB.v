module stim_1_e;
wire q,q_bar;
reg d,clk;
dflipflop_fall dflipflop1(q,q_bar,d,clk);
initial
begin
d = 1'b0; clk=1'b0;
$monitor("d=%b,clk=%b,q=%b,q_bar=%b",d,clk,q,q_bar);
#5 d = 1'b1;
#5 clk = 1'b1;
#10 clk = 1'b0;
#5 d = 1'b0;
#5 clk = 1'b1;
#10 clk = 1'b0;
#5 d = 1'b1;
#5 clk = 1'b1;
#5 d = 1'b0;
#5 clk = 1'b0;
#10 clk = 1'b1;
#5 d = 1'b1;
end
endmodule
