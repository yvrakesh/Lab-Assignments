module stim_mux_1_2;
wire y;
reg s,a,b;
mux_1_2 mux1(y,s,a,b);
initial
begin
	$monitor("s=%b,a=%b,b=%b,y=%b",s,a,b,y);
	#10 s=0;a=0;b=0;
	#10 s=1;a=0;b=0;
	#10 s=0;a=0;b=1;
	#10 s=1;a=0;b=1;
	#10 s=0;a=1;b=0;
	#10 s=1;a=1;b=0;
	#10 s=0;a=1;b=1;
	#10 s=1;a=1;b=1;
end
endmodule