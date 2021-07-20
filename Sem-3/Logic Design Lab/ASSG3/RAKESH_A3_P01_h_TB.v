module stim_1_h;
reg d,clk,r;
wire q,q1;
dff_f_asrl d1(q,q1,d,clk,r);
initial
begin
clk=0;d=0;r=0;
end
always
begin
#10 clk=~clk;
end
always
begin
#12 r=~r;
end
always
begin
$monitor("Q=%b, Q_=%b, D=%b,clk=%b",q,q1,d,clk);
#8 d=~d;
end
endmodule