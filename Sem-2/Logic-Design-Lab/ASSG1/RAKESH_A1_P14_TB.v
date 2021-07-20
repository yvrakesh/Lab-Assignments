module stim_mux_16;
wire [15:0]y;
reg [15:0]s,a,b;
mux_1_2 mux2[15:0](y,s,a,b);
integer i,j,k;
initial
begin
	for(i=0;i<2;i=i+1)
	begin
	  for(j=0;j<2;j=j+1)
	  begin 
	    for(k=0;k<2;k=k+1)
	    begin
		#10 s=i;a=j;b=k;
	$monitor("s=%b,a=%b,b=%b,y=%b",s,a,b,y);
	    end
	   end
	  end
end
endmodule
