module TM;

reg clk, sel;
reg [3:0] a, b;
wire [3:0] sum ;
wire cout ;

AddSub4 addsub4(a, b, sum, cout, sel);

initial clk = 1'd0;
always #5 clk = ~clk;

initial
begin
	a = 4'd0;
	b = 4'd0;
	sel = 0;

#5  a = 4'd5;
	b = 4'd1;
	sel = 0;
	
#5  a = 4'd5;
	b = 4'd1;
	sel = 1;

#5  a = 4'd0;
	b = 4'd0;
	sel = 0;
	
#5  a = 4'd0;
	b = 4'd0;
	sel = 1;
	
#5  a = 4'd2;
	b = 4'd2;
	sel = 0;
	
#5  a = 4'd2;
	b = 4'd2;
	sel = 1;
	
#5  a = 4'd1;
	b = 4'd5;
	sel = 0;
	
#5  a = 4'd1;
	b = 4'd5;
	sel = 1;
end
endmodule