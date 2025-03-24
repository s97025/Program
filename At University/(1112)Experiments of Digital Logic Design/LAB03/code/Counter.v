module Counter(dout, clk, rst, en, up);

	input clk, rst, en, up;
	output reg[7:0] dout;

	always @(posedge clk) begin
		if (rst == 0) dout = 0 ;
		
		else begin
			if (en == 1) begin
			
				if (up == 0) begin
					if ( dout != 8'd0 )
						dout = dout - 1 ;
				end
				else begin
					if ( dout != 8'd255 )
						dout = dout + 1 ;
				end
			end
		end
	end


endmodule

