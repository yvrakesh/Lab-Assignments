module stim_4;

wire [15:0]out;
reg [15:0]in;
reg [2:0]add;
reg clk,read,write,en;

RAM8_16bit RAM(out,in,clk,read,write,add,en);

initial clk=1'b0;
always #5 clk=~clk;

initial
begin
	$monitor("clk=%b,read=%b,write=%b,add=%b,en=%b,in=%b,out=%b",clk,read,write,add,en,in,out);
	read=1'b1;write=1'b1;en=1'b1;add=3'b000;in=1;#10;
	read=1'b1;write=1'b1;en=1'b1;add=3'b001;in=2;#10;
	read=1'b1;write=1'b1;en=1'b1;add=3'b010;in=3;#10;
	read=1'b1;write=1'b1;en=1'b1;add=3'b011;in=4;#10;
	read=1'b1;write=1'b1;en=1'b1;add=3'b100;in=5;#10;
	read=1'b1;write=1'b1;en=1'b1;add=3'b101;in=6;#10;
	read=1'b1;write=1'b1;en=1'b1;add=3'b110;in=7;#10;
	read=1'b1;write=1'b1;en=1'b1;add=3'b111;in=8;#10;
	read=1'b1;write=1'b1;en=1'b0;add=3'b101;in=9;#10;
	read=1'b1;write=1'b1;en=1'b1;add=3'b100;in=10;#10;
end

endmodule


