
module stim_3;
wire [15:0]d_out;
reg [15:0]d_in;
reg cs,w,r,clk;
sixteenbitregister one1(d_out,d_in,cs,w,r,clk);
initial
begin
$monitor("d_out=%b,d_in=%b,cs=%b,w=%b,r=%b,clk=%b",d_out,d_in,cs,w,r,clk);
d_in=127 ;clk=1'b0; r = 1'b0; w =1'b0; cs=1'b1;
#10 d_in=255 ;clk=1'b0; r =1'b0; w=1'b1;
#10 d_in=511 ;clk=1'b1; r =1'b1;w =1'b0;
#10 d_in=1023 ;clk=1'b1; r =1'b1; w=1'b1;#10;
end
endmodule
