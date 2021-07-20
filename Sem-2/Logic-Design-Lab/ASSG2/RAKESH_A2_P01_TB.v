module stim_Half_Adder;
wire s,c;
reg a,b;
Half_Adder Half_Adder_1(s,c,a,b);
initial
begin
	$monitor("a=%b,b=%b,s=%b,c=%b",a,b,s,c);
	#10 a=1'b0;b=1'b0;
	#10 a=1'b0;b=1'b1;
	#10 a=1'b1;b=1'b0;
	#10 a=1'b1;b=1'b1;
end
endmodule
