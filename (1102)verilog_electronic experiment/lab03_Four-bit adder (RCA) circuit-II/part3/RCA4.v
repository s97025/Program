module RCA4( a, b, cout, s ) ;

 input [3:0] a, b ;
 output [3:0] s ;
 output cout ;
 
 wire [4:0] at, bt ;
 wire [4:0] st ;
 
 assign at = {1'b0, a} ;
 assign bt = {1'b0, b} ;
 assign st = at + bt ;
 assign s = st[3:0] ;
 assign cout = st[4:4] ;
 
 endmodule
 