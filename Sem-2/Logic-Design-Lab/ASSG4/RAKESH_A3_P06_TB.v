
module stim_6;

wire [15:0]d_out;
reg [15:0]d_in;
reg [8:0]add;
reg clk,r,w,en;
integer i,j;

ram512 RAM(d_out,d_in,w,r,en,clk,add);

initial clk=1'b0;
always #5 clk=~clk;

initial
begin
	$monitor("clk=%b,r=%b,w=%b,en=%b,add=%b,d_in=%b,d_out=%b",clk,r,w,en,add,d_in,d_out);
	for(i=2;i<512;i=i+64)
	begin
		r=1'b1;w=1'b1;en=1'b1;add=i;d_in=i;#10;
	end
	for(j=2;j<512;j=j+64)
	begin
		r=1'b1;w=1'b0;en=1'b1;add=j;#10;
	end
end

endmodule
