module stim_mux_8_1_16;
wire [15:0]c;
reg [15:0]i1,i2,i3,i4,i5,i6,i7,i8,s1,s2,s3;
mux_8_1_16 mux_8_16[15:0](c,i1,i2,i3,i4,i5,i6,i7,i8,s1,s2,s3);
integer j1,j2,j3,j4,j5,j6,j7,j8,j9,j10,j11;
initial
begin
	for(j1=0;j1<2;j1=j1+1)
	begin
	 for(j2=0;j2<2;j2=j2+1)
	 begin
	  for(j3=0;j3<2;j3=j3+1)
	  begin
	   for(j4=0;j4<2;j4=j4+1)
	   begin
	    for(j5=0;j5<2;j5=j5+1)
	    begin
	     for(j6=0;j6<2;j6=j6+1)
	     begin
	      for(j7=0;j7<2;j7=j7+1)
	      begin
		for(j8=0;j8<2;j8=j8+1)
	        begin
		 for(j9=0;j9<2;j9=j9+1)
		 begin
		  for(j10=0;j10<2;j10=j10+1)
		  begin
	   	   for(j11=0;j11<2;j11=j11+1)
		   begin
#10 i1=j1;i2=j2;i3=j3;i4=j4;i5=j5;i6=j6;i7=j7;i8=j8;s1=j9;s2=j10;s3=j11;
$monitor("i1=%b,i2=%b,i3=%b,i4=%b,i5=%b,i6=%b,i7=%b,i8=%b,s1=%b,s2=%b,s3=%b,c=%b",i1,i2,i3,i4,i5,i6,i7,i8,s1,s2,s3,c);
		   end
		  end
		 end
		end
	      end
	     end
	    end
	   end
	  end
	 end
	end
end
endmodule
