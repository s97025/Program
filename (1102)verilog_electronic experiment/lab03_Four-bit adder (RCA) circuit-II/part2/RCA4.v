module RCA4( a, b, s ) ;

 input [3:0] a, b ;
 output [3:0] s ;
 
 wire [3:0] c ;
 
 assign s = a + b ;
 
 endmodule
 