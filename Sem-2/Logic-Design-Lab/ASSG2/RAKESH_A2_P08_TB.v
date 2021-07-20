module stim_alu;
wire [15:0]c;
wire zr,ng;
reg [15:0]a,b;
reg [5:0]s;
alu alu_1(c,zr,ng,a,b,s);
integer i,j,k;
initial
begin
	for(k=0;k<63;k=k+1)
	begin
     $monitor("a=%b,b=%b,c=%b,s=%b,zr=%b,ng=%b",a,b,c,s,zr,ng);
	#10 a=10;b=11;s=k;
	end
end
endmodule
