module stim_Adder_16;
wire [15:0]c;
wire carry;
reg [15:0]a,b;
Adder_16 Adder_16_1(c,carry,a,b);
integer i,j;
initial
begin
	for(i=0;i<10;i=i+1)
	begin
	   for(j=0;j<10;j=j+1)
	   begin
		#10 a=i;b=j;
$monitor("a=%b,b=%b,c=%b,carry=%b",a,b,c,carry);
	   end
	end

end
endmodule
