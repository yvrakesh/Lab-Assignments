module stim_Adder_4;
wire [3:0]c;
wire carry;
reg [3:0]a,b;
Adder_4 Adder_4_1(c,carry,a,b);
integer i,j;
initial
begin
	for(i=0;i<16;i=i+1)
	begin
	   for(j=0;j<16;j=j+1)
	   begin
		#10 a=i;b=j;
$monitor("a=%b,b=%b,c=%b,carry=%b",a,b,c,carry);
	   end
	end
end
endmodule
