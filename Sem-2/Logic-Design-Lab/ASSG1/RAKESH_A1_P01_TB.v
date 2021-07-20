module stim_not_gate;
wire b;
reg a;
not_gate not_gate1(b,a);
initial
begin
   $monitor("a = %b,b = %b",a,b);
	a = 1'b0;
    #10 a = 1'b1;
end
endmodule