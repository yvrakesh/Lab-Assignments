module stim_demux;
wire o1,o2;
reg i,s;
demux_1_2 demux1(o1,o2,i,s);
initial
begin
	$monitor("i=%b,s=%b,o1=%b,o2=%b",i,s,o1,o2);
	#10 i=0;s=0;
	#10 i=0;s=1;
	#10 i=1;s=0;
	#10 i=1;s=1;
end
endmodule
