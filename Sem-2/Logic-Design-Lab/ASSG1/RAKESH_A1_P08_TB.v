module stim_and_gate_16;
wire [15:0]c;
reg [15:0]a;
reg [15:0]b;
integer i,j;
and_gate and_gate_2[15:0](c,a,b);
always
begin
	i = 0;
	while(i<10)
	begin
	     j=0;
		while(j<10)
		begin
		
			#10 a = i;
			    b = j;
		$monitor("a = %b,b = %b,c = %b",a,b,c);
		     j = j+1;
		end
	 i = i+1;
	end
end
endmodule
		
