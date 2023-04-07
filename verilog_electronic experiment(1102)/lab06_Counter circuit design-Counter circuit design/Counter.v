module Counter(  clk, rst ,dout) ;
input clk ;
output reg[3:0] dout;
input[3:0]   rst;
always@(posedge clk) begin
   if (rst==0)
      dout <= 4'b0000 ;
   else
      dout <= dout + 4'b0001 ;
end
endmodule