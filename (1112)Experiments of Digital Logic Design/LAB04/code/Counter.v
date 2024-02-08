module Counter(dout, clk, rst, en, up);

	input clk, rst, en, up;
	output reg[7:0] dout;

	always @(posedge clk or negedge rst) begin
		if (rst == 0) dout = 0 ;
		
		else begin
			case( {en, up} )
				2'b11: 	begin
							if ( dout != 8'd255 )
								dout = dout + 1 ;
						end
						
				2'b10: 	begin
							if ( dout != 8'd0 )
								dout = dout - 1 ;
						end
			endcase
		end
	end
endmodule

