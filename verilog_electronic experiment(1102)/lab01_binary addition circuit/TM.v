`timescale 1ns/1ns
module TM ;
   
reg A, B, Cin ;
wire Sum, Cout ;

FA   U_FA (.a(A), .b(B), .c(Cin), .cout(Cout), .sum(Sum));

parameter	t = 200;
initial 
begin
      #(2*t)
        A = 1'b0;
        B = 1'b0;
        Cin = 1'b0;
	
      #t 	
        A = 1'b0;
        B = 1'b0;
        Cin = 1'b1;
        
      #t 	
        A = 1'b0;
        B = 1'b1;
        Cin = 1'b0;
        
      #t 	
        A = 1'b0;
        B = 1'b1;
        Cin = 1'b1;
        
      #t 	
        A = 1'b1;
        B = 1'b0;
        Cin = 1'b0;
        
      #t 	
        A = 1'b1;
        B = 1'b0;
        Cin = 1'b1;
        
      #t 	
        A = 1'b1;
        B = 1'b1;
        Cin = 1'b0;
        
      #t 	
        A = 1'b1;
        B = 1'b1;
        Cin = 1'b1;
        
      #t
      $stop ;
    end
    
endmodule

