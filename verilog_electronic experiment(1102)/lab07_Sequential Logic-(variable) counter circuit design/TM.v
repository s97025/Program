module TM;

reg            sel,clk, rst ;
reg[3:0] a, b;
wire [3:0]     dout;

parameter      t = 200;
parameter      th= 100;

Counter_sel    U_Counter_sel  ( .a(a), .b(b), .sel(sel), .clk(clk), .rst(rst), .dout(dout) );

always #th  clk=~clk;
initial  begin
  rst = 1;
  sel = 1 ;
  clk =0 ;
  a = 4'b0000 ;
  b = 4'b0000 ;
           
  #t rst = 0 ;         
  #t rst = 1 ;
	#(t*13) sel = 0 ;
	a = 4'b0101 ;
	b = 4'b1001 ;
	#t
	a = 4'b1011 ;
	b = 4'b0010 ; 

  #t $stop;
 end
endmodule

