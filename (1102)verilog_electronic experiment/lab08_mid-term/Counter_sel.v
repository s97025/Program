module Counter_sel( a, b, sel, clk, rst ,dout ) ;
input clk, rst ; 
input[1:0] sel ; 
input[2:0] a, b ;
output reg[3:0] dout;


always@(posedge clk) begin
  
   if ( rst == 0 )
      dout <= 4'b0000 ;
      
   else
        
      if ( sel == 2'b01 )  
        dout <= dout + 4'b0001 ;
               
      else if ( sel == 2'b00 )  
        dout <= dout ;
            
      else if ( sel == 2'b10 ) 
        dout <=  { 1'b0, a } + { 1'b0, b } ;
                    
end
endmodule

