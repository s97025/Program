module TM;

reg            clk ;
reg[3:0] rst;
wire [3:0]     dout;

parameter      t = 200;
parameter      th= 100;

Counter    U_cnt  ( .clk(clk), .rst(rst), .dout(dout) );

always #th  clk=~clk;
initial  begin
  rst = 1;
  clk =0 ;
           
  #t rst = 0 ;         
  #t rst = 1 ;
	#(t*14) 


  #t $stop;
 end
endmodule
