module stim_Incrementor_4;
wire [3:0]b;
reg [3:0]a;
Incrementor_4 Incrementor_4_1(b,a);
integer i;
initial
begin
	for(i=0;i<=15;i=i+1)
	begin
	#10 a=i;
	$monitor("a=%b,b=%b",a,b);
end
end
endmodule
		
