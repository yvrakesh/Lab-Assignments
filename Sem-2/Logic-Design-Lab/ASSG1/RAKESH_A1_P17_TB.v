module stim_demux_1_4;
wire o1,o2,o3,o4;
reg i1,s1,s2;
demux_1_4 demux_4(o1,o2,o3,o4,i1,s1,s2);
integer i,j,k;
initial
begin
	for(i=0;i<2;i=i+1)
	begin
	   for(j=0;j<2;j=j+1)
	   begin
	      for(k=0;k<2;k=k+1)
	      begin
		#10 i1=i;s1=j;s2=k;
	$monitor("i1=%b,s1=%b,s2=%b,o1=%b,o2=%b,o3=%b,o4=%b",i1,s1,s2,o1,o2,o3,o4);
	       end
	   end
	end
end
endmodule
