module stim_mux_4_1_16;
wire [15:0]y;
reg [15:0]i1,i2,i3,i4,s1,s2;
mux_4_1 mux_4_16[15:0](y,i1,i2,i3,i4,s1,s2);
integer i,j,k,l,m,n;
always
begin
	
	for(i=0;i<2;i=i+1)
	begin
	 for(j=0;j<2;j=j+1)
	 begin
	  for(k=0;k<2;k=k+1)
	  begin
	   for(l=0;l<2;l=l+1)
	   begin
	    for(m=0;m<2;m=m+1)
	    begin
	     for(n=0;n<2;n=n+1)
	     begin
		#10 i1=i;i2=j;i3=k;i4=l;s1=m;s2=n;
$monitor("i1=%b,i2=%b;i3=%b,i4=%b,s1=%b,s2=%b,y=%b",i1,i2,i3,i4,s1,s2,y);
	     end
	    end
	   end
	  end
	 end
	end
end
endmodule
