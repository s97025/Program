module AddSub4(a, b, sum, cout, sel);

input sel;
input [3:0] a, b;
output [3:0] sum;
output cout;
wire [4:0] result, tmpA, tmpB ;

assign tmpA = {1'b0, a} ;
assign tmpB = (sel == 1'd0) ? {1'd0, b} : {1'd0, ~b}+1'd1 ;
assign result = tmpA + tmpB ;
assign sum = result[3:0];
assign cout = result[4];


endmodule

