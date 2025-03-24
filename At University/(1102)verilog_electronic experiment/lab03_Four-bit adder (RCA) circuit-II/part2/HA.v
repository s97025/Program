`timescale 1ns/1ns
module HA(a, b, c, cout, sum) ;

  input a, b, c ;
  output cout, sum ;
  
  xor(sum, a, b) ;

  and(cout, a, b) ;
 
  
endmodule


