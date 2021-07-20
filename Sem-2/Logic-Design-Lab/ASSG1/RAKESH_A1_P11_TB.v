module stim_or_gate_8;
wire c;
reg i1,i2,i3,i4,i5,i6,i7,i8;
or_gate_8 or_gate3(c,i1,i2,i3,i4,i5,i6,i7,i8);
integer j1,j2,j3,j4,j5,j6,j7,j8;
initial
begin
	/* #10 i1=0;i2=0;i3=0;i4=1;i5=0;i6=1;i7=0;i8=1;
	#10 i1=0;i2=1;i3=0;i4=1;i5=1;i6=0;i7=0;i8=1;
	#10 i1=0;i2=0;i3=0;i4=0;i5=0;i6=0;i7=0;i8=0; */
	for( j1=0;j1<2;j1=j1+1)
	begin
	for( j2=0;j2<2;j2=j2+1)
	begin
	for( j3=0;j3<2;j3=j3+1)
	begin
	for( j4=0;j4<2;j4=j4+1)
	begin
	for( j5=0;j5<2;j5=j5+1)
	begin
	for( j6=0;j6<2;j6=j6+1)
	begin	
	for( j7=0;j7<2;j7=j7+1)
	begin
	for( j8=0;j8<2;j8=j8+1)
	begin
		$monitor("i1=%b,i2=%b,i3=%b,i4=%b,i5=%b,i6=%b,i7=%b,i8=%b,c=%b",i1,i2,i3,i4,i5,i6,i7,i8,c);
		#10 i1=j1;i2=j2;i3=j3;i4=j4;i5=j5;i6=j6;i7=j7;i8=j8;
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
