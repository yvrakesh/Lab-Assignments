module stim_5;

wire [15:0]out;
reg [15:0]in;
reg [5:0]add;
reg clk,read,write,en1;

RAM64_16bit RAM(out,in,clk,read,write,add,en1);

initial clk=1'b0;
always #5 clk=~clk;

initial
begin
	$monitor("clk=%b,read=%b,write=%b,en1=%b,add=%b,in=%b,out=%b",clk,read,write,en1,add,in,out);
	read=1'b1;write=1'b1;en1=1'b1;add=2;in=1;#10;
	read=1'b1;write=1'b1;en1=1'b1;add=10;in=2;#10;
	read=1'b1;write=1'b1;en1=1'b1;add=18;in=3;#10;
	read=1'b1;write=1'b1;en1=1'b1;add=26;in=4;#10;
	read=1'b1;write=1'b1;en1=1'b1;add=34;in=5;#10;
	read=1'b1;write=1'b1;en1=1'b1;add=42;in=6;#10;
	read=1'b1;write=1'b1;en1=1'b1;add=50;in=7;#10;
	read=1'b1;write=1'b1;en1=1'b1;add=58;in=8;#10;
	read=1'b1;write=1'b1;en1=1'b1;add=32;in=9;#10;
	read=1'b1;write=1'b1;en1=1'b1;add=32;in=10;#10;
end

endmodule


