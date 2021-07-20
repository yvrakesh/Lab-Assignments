module stim_not_gate_16;
wire [15:0]b;
reg [15:0]a;
integer i;
not_gate not_gate2[15:0](b,a);
initial
begin
	i=0;
	while(i<65535)
begin
	   a = i;
	$monitor("a = %b, b = %b",a,b);
	#10 i = i+1;
end
end
endmodule
