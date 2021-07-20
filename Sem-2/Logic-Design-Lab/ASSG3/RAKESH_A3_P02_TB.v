
module stim_2;
wire d_out;
reg d_in,cs,w,r,clk;
onebitbinarycell one(d_out,d_in,cs,w,r,clk);
initial
begin
$monitor("d_out=%b,d_in=%b,cs=%b,w=%b,r=%b,clk=%b",d_out,d_in,cs,w,r,clk);
d_in=1'b0 ;clk=1'b0; r = 1'b0; w =1'b0; cs=1'b1;
#10 d_in=1'b1 ;clk=1'b0; r =1'b0; w=1'b1;
#10 d_in=1'b1 ;clk=1'b1; r =1'b1;w =1'b0;
#10 d_in=1'b0 ;clk=1'b1;#10; r =1'b1; w=1'b1;
end
endmodule
