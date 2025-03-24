`timescale 1ns/1ns
module FA(a, b, c, cout, sum) ;

  input a, b, c ;
  output cout, sum ;
  
  wire e1, e2, e3, e4, e5 ;
  
  and(e1, a, b) ;
  and(e2, a, c) ;
  and(e3, b, c) ;
  or(e4, e1, e2) ;
  or(cout, e3, e4) ;
  xor(e5, a, b) ;
  xor(sum, e5, c) ;
  
endmodule
