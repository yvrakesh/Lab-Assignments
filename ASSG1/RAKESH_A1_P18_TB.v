module stim_demux_1_8;
wire o1,o2,o3,o4,o5,o6,o7,o8;
reg i1,s1,s2,s3;
demux_1_8 demux_8(o1,o2,o3,o4,o5,o6,o7,o8,i1,s1,s2,s3);
integer i,j,k,m;
initial
begin
	for(i=0;i<2;i=i+1)
	begin
	 for(j=0;j<2;j=j+1)
	 begin
	  for(k=0;k<2;k=k+1)
	  begin
	   for(m=0;m<2;m=m+1)
	   begin
		#10 i1=i;s1=j;s2=k;s3=m;
$monitor("i1=%b,s1=%b,s2=%b,s3=%b,o1=%b,o2=%b,o3=%b,o4=%b,o5=%b,o6=%b,o7=%b,o8=%b",i1,s1,s2,s3,o1,o2,o3,o4,o5,o6,o7,o8);
	   end
	  end
	 end
	end
end
endmodule
