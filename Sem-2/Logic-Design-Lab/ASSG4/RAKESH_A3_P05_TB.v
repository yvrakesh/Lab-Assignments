module stim_5;

wire [15:0]d_out;
reg [15:0]d_in;
reg [5:0]add;
reg clk,r,w,en;

ram64 RAM(d_out,d_in,w,r,en,clk,add);

initial clk=1'b0;
always #5 clk=~clk;

initial
begin
	$monitor("clk=%b,r=%b,w=%b,en=%b,add=%b,d_in=%b,d_out=%b",clk,r,w,en,add,d_in,d_out);
	r=1'b1;w=1'b1;en=1'b1;add=2;d_in=1;#10;
	r=1'b1;w=1'b1;en=1'b1;add=10;d_in=2;#10;
	r=1'b1;w=1'b1;en=1'b1;add=18;d_in=3;#10;
	r=1'b1;w=1'b1;en=1'b1;add=26;d_in=4;#10;
	r=1'b1;w=1'b1;en=1'b1;add=34;d_in=5;#10;
	r=1'b1;w=1'b1;en=1'b1;add=42;d_in=6;#10;
	r=1'b1;w=1'b1;en=1'b1;add=50;d_in=7;#10;
	r=1'b1;w=1'b1;en=1'b1;add=58;d_in=8;#10;
	r=1'b1;w=1'b1;en=1'b1;add=32;d_in=9;#10;
	r=1'b1;w=1'b1;en=1'b1;add=32;d_in=10;#10;
end

endmodule


