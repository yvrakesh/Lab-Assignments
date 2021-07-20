module stim_Full_Adder;
wire s,cout;
reg a,b,cin;
Full_Adder Full_Adder_1(s,cout,a,b,cin);
initial
begin
	$monitor("a=%b,b=%b,cin=%b,s=%b,cout=%b",a,b,cin,s,cout);
	#10 a=1'b0;b=1'b0;cin=1'b0;
	#10 a=1'b0;b=1'b0;cin=1'b1;
	#10 a=1'b0;b=1'b1;cin=1'b0;
	#10 a=1'b0;b=1'b1;cin=1'b1;
	#10 a=1'b1;b=1'b0;cin=1'b0;
	#10 a=1'b1;b=1'b0;cin=1'b1;
	#10 a=1'b1;b=1'b1;cin=1'b0;
	#10 a=1'b1;b=1'b1;cin=1'b1;
end
endmodule
