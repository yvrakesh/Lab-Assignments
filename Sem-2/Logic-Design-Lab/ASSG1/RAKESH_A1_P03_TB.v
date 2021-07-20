module stim_or_gate;
wire c;
reg a,b;
or_gate or_gate1(c,a,b);
initial
begin
    $monitor("a = %b,b = %b,c = %b",a,b,c);
	#10 a = 1'b0; b = 1'b0;
	#10 a = 1'b0; b = 1'b1;
	#10 a = 1'b1; b = 1'b0;
	#10 a = 1'b1; b = 1'b1;
end
endmodule
