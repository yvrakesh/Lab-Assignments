module stim_Incrementor_16;
wire [15:0]b;
wire carry;
reg [15:0]a;
Incrementor_16 Incrementor_16_1(b,carry,a);
integer i;
initial
begin
	for(i=65533;i<=65535;i=i+1)
	begin
		#10 a=i;
$monitor("a=%b,b=%b,carry=%b",a,b,carry);
	end
end
endmodule
