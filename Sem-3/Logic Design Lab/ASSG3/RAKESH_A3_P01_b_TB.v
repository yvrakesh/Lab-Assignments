module stim_1_b;
wire q,q_bar;
reg d,clk,reset;
dflipflop_rise_syn dflipflop1(q,q_bar,d,clk,reset);
initial
begin
reset = 0;
d = 1'b1; clk = 1'b0;
$monitor("d=%b,clk=%b,q=%b,q_bar=%b,reset=%b",d,clk,q,q_bar,reset);
#5 d = 1'b0;
#5 clk = 1'b1;
#10 clk = 1'b0;
#5 d = 1'b1;
#5 clk = 1'b1;
#5 d = 1'b0;
reset = 1;
#5 clk = 1'b0;
#5 d = 1'b1;
#5 clk = 1'b1;
end
endmodule
