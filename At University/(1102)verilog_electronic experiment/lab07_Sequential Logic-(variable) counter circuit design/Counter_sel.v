module Counter_sel(  a, b, sel, clk, rst ,dout) ;
input clk, rst, sel ; 
input[3:0] a, b ;
output reg[3:0] dout;

always@(posedge clk) begin
  
   if (rst==0)
      dout <= 4'b0000 ;
      
   else
      if ( sel == 0 )
        dout <= a&b;
       
      else if ( sel == 1 )  
        dout <= dout + 1 ;
      
end
endmodule

