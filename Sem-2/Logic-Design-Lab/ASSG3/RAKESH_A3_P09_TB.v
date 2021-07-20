
module stim_9;
reg [15:0] d_i;
reg res,i,clk,load;
wire [15:0] d_o;
pc pc1(d_o,d_i,i,load,res,clk);
initial
begin
$monitor("OUT=%b",d_o);
clk=0;

#10 d_i=16'b0000000000111111;res=1;i=0;load=0;
#10 ;res=0;i=1;
#10 ;res=0;i=1;
#10 ;res=0;i=1;
#10 res=0;load=1;
#10 res=0;load=0;
#10 ;res=0;i=1;
#10 ;res=0;i=1;
#10 ;res=0;i=0;
#10 ;res=0;i=0;
#10 ;res=0;i=0;
#10 ;res=0;i=1;
#10 res=0;load=1;d_i=16'b0000000000000011;i=0;
#10 load=0;res=0;i=1;
#10 ;res=0;i=1;
#10 res=0;i=1;
#10 res=1;i=0;
#10 res=0;i=1;
#10 res=0;i=1;
end
always 
begin 
#5 clk=~clk;
end
endmodule