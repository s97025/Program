module MUX_4to1( out, in0, in1, in2, in3, sel ) ;

output out ;
input in0, in1, in2, in3 ;
input sel ;

wire out ;

assign out = ( sel == 2'b00 ) ? in0 : ( sel == 2'b01 ) ? in1 : ( sel == 2'b10 ) ? in2 : in3 ;

endmodule