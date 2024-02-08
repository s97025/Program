module mux_4_to_1_if( out, in0, in1, in2, in3, sel ) ;

output out ;
input in0, in1, in2, in3 ;
input sel ;

reg out ;

always @ ( sel or in0 or in1 or in2 or in3 )
begin
  if ( sel == 2'b00 )
    out = in0 ;
  else if ( sel == 2'b01 )
    out = in1 ;
  else if ( sel == 2'b10 )
    out = in2 ;
  else if ( sel == 2'b11 )
    out = in3 ;
    
 end 

endmodule

