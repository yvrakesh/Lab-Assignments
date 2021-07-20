
module stim_9;
reg [15:0] d_in;
reg reset,inc,clk,load;
wire [15:0] d_out;
pc pc1(d_out,d_in,inc,load,reset,clk);
initial
begin
$monitor("d_out=%b",d_out);
clk=0;

#10 d_in=16'b0000000000111111;reset=1;inc=0;load=0;
#10 reset=0;inc=1;
#10 reset=0;inc=1;
#10 reset=0;inc=1;
#10 reset=0;load=1;
#10 reset=0;load=0;
#10 reset=0;inc=1;
#10 reset=0;inc=1;
#10 reset=0;inc=0;
#10 reset=0;inc=0;
#10 reset=0;inc=0;
#10 reset=0;inc=1;
#10 reset=0;load=1;d_in=16'b0000000000000011;inc=0;
#10 load=0;reset=0;inc=1;
#10 reset=0;inc=1;
#10 reset=0;inc=1;
#10 reset=1;inc=0;
#10 reset=0;inc=1;
#10 reset=0;inc=1;
end
always 
begin 
#5 clk=~clk;
end
endmodule
