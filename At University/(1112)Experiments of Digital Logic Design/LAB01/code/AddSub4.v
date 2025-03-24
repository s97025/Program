module FA(a, b, c, sum, cout);
  
	input a, b, c;
	output cout, sum;
	wire tmpAB, cANDtmpAB, aANDb;
  
	assign tmpAB = a ^ b;
	assign cANDtmpAB = c & tmpAB;
	assign aANDb = a & b;
	assign sum = tmpAB ^ c;
	assign cout = cANDtmpAB | aANDb;
endmodule
 
 
module AddSub(a, b, sum, cout, sel);

	output [3:0] sum ;
	output cout ;	
	input [3:0] a, b;
	input sel;
	wire[3:0] c, tmpB;

	assign tmpB[0] = ( sel == 0 ) ? b[0] : (b[0] ^ sel) ;
	assign tmpB[1] = ( sel == 0 ) ? b[1] : (b[1] ^ sel) ;
	assign tmpB[2] = ( sel == 0 ) ? b[2] : (b[2] ^ sel) ;
	assign tmpB[3] = ( sel == 0 ) ? b[3] : (b[3] ^ sel) ;	
	
	FA fa0(a[0], tmpB[0], sel, sum[0], c[0]);
	FA fa1(a[1], tmpB[1], c[0], sum[1], c[1]);
	FA fa2(a[2], tmpB[2], c[1], sum[2], c[2]);
	FA fa3(a[3], tmpB[3], c[2], sum[3], cout);
	
	
endmodule