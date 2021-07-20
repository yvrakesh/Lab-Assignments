module stim_4;

wire [15:0]d_out;
reg [15:0]d_in;
reg [2:0]add;
reg clk,r,w,en;

ram8 RAM(d_out,d_in,w,r,en,clk,add);

initial clk=1'b0;
always #5 clk=~clk;

initial
begin
	$monitor("clk=%b,r=%b,w=%b,add=%b,en=%b,d_in=%b,d_out=%b",clk,r,w,add,en,d_in,d_out);
	r=1'b1;w=1'b1;en=1'b1;add=3'b000;d_in=1;#10;
	r=1'b1;w=1'b1;en=1'b1;add=3'b001;d_in=2;#10;
	r=1'b1;w=1'b1;en=1'b1;add=3'b010;d_in=3;#10;
	r=1'b1;w=1'b1;en=1'b1;add=3'b011;d_in=4;#10;
	r=1'b1;w=1'b1;en=1'b1;add=3'b100;d_in=5;#10;
	r=1'b1;w=1'b1;en=1'b1;add=3'b101;d_in=6;#10;
	r=1'b1;w=1'b1;en=1'b1;add=3'b110;d_in=7;#10;
	r=1'b1;w=1'b1;en=1'b1;add=3'b111;d_in=8;#10;
	r=1'b1;w=1'b1;en=1'b0;add=3'b101;d_in=9;#10;
	r=1'b1;w=1'b1;en=1'b1;add=3'b100;d_in=10;#10;
end

endmodule
