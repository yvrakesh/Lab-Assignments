module stim_6;

wire [15:0]out;
reg [15:0]in;
reg [8:0]add;
reg clk,read,write,en1;
integer i,j;

RAM512_16bit RAM(out,in,clk,read,write,add,en1);

initial clk=1'b0;
always #5 clk=~clk;

initial
begin
	$monitor("clk=%b,read=%b,write=%b,en1=%b,add=%b,in=%b,out=%b",clk,read,write,en1,add,in,out);
	for(i=2;i<512;i=i+64)
	begin
		read=1'b1;write=1'b1;en1=1'b1;add=i;in=i;#10;
	end
	for(j=2;j<512;j=j+64)
	begin
		read=1'b1;write=1'b0;en1=1'b1;add=j;#10;
	end
end

endmodule

