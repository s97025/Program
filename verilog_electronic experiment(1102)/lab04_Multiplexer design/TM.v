module TM_Mux_4to1;

reg    in0, in1, in2, in3;
reg   [1:0]  sel;
wire  out;

MUX_4to1     U_Mux_4to1 (.out(out), .in0(in0), .in1(in1), .in2(in2), .in3(in3), .sel(sel) );

parameter	t = 200;
initial 
begin
             #t
             in0 = 1'b0;
             in1 = 1'b1;
             in2 = 1'b0;
             in3 = 1'b1;
	
             #t 	
             sel = 2'b00 ;
             
             #t
             sel = 2'b01 ;
             
             #t
             sel = 2'b10 ;
             
             #t
             sel = 2'b11 ;
             
           #t  $stop;
 end

endmodule
