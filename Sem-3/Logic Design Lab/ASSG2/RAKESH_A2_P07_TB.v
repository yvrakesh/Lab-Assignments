module stim_negator_16;
wire [15:0]b;
reg [15:0]a;
Negator_16 Negator_16_1(b,a);
integer i;
initial
begin
	for(i=65520;i<65535;i=i+1)
	begin
	  #10 a=i;
$monitor("a=%b,b=%b",a,b);
	end
end
endmodule
