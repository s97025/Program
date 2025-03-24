`timescale 1 ns / 10 ps

module Counter_TM;

parameter	WIDTH = 8;
parameter	bit_width = 256;


reg					clk, rst, en, up;
wire	[WIDTH-1:0] dout;


Counter		U_Counter ( 

.dout	(dout),

.clk	(clk),
.rst	(rst),
.en		(en), 
.up		(up)
);


//*********************************
//      control signal
//*********************************
parameter   t   = 10;
parameter   th  = t*0.5;
reg	[4:0]	cnt;
always #th clk = ~clk;

initial begin
    clk = 1;
    rst = 1;
    cnt = 0;
    en = 0;
    up = 1;
    #th rst = 0;
    #(t*2)      rst = 1;
    #t
    	en = 1;
    #(t*(bit_width+10))     
    	if(dout!=8'hff)	
    	begin 
    		$display("\n There is error occured in up counter!! \n");
    		cnt = cnt + 1;
    	end
    #t          
    	up = 0;
    #(t*(bit_width+10))     
    	if(dout!=8'h00)	
    	begin
    		$display("\n There is error occured in down counter!! \n");
    		cnt = cnt + 1;
    	end
    	
    #t	
    	up = 1;
    	en = 0;
    	
    #(t*100)		
    	if(dout!=8'h00)	
    	begin
    		$display("\n There is error occured in enable control!! \n");
    		cnt = cnt + 1;
    	end
    	
    	up = 0;
    #(t*10)		
    	if(dout!=8'h00)
    	begin
    		$display("\n There is error occured in enable control!! \n");
    		cnt = cnt + 1;
    	end
    	    
    #t
		up = 1;
		en = 1;
	#(t*(bit_width-100))
		if(dout!=8'd156)	
		begin
			$display("\n There is error occured in this counter with state1!! \n");
			cnt = cnt + 1;
		end
		
		up = 0;
	#(t*(75))
		en = 0;
		if(dout!=8'd81)	
		begin
			$display("\n There is error occured in this counter with state2!! \n");
			cnt = cnt + 1;
		end
    #t      
    	if(cnt == 0)	
			$display("\nPass......Pass......Pass......Pass......Pass......!! \n");
		else
			$display("\nFail......Fail......Fail......Fail......Fail......!! \n");
    
    $stop;
end

endmodule
