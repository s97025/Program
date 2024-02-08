module TM;

reg clk, rst ;
reg[1:0] sel ;
reg[2:0] a, b;
wire [3:0] dout;


parameter      t = 200;
parameter      th= 100;

Counter_sel    U_Counter_sel  ( .a(a), .b(b), .sel(sel), .clk(clk), .rst(rst), .dout(dout) );

always #th  clk=~clk;
initial  begin
  rst = 1 ;
  sel = 2'b00 ;
  clk = 0 ;
  a = 3'b000 ;
  b = 3'b000 ;
      
 	#t rst = 0;
 	#t rst = 1;
 	                         
	#t sel = 2'b01 ;	
	
	#(t*12)sel = 2'b00 ;	
	
	
	#(t*2) sel = 2'b10 ;	
	a = 3'b101 ;
	b = 3'b100 ;
	#t
	a = 3'b001 ;
	b = 3'b011 ; 

  #t $stop;
 end
endmodule

