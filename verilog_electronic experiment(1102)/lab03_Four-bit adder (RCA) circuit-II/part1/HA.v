`timescale 1ns/1ns
module HA(a, b, c, cout, sum) ;

  input a, b, c ;
  output cout, sum ;
  
  assign sum = a ^ b ;
  assign cout = a & b ;

  
endmodule


