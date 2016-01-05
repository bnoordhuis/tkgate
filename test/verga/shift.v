module top;
	reg [127: 0] big;
	reg [31:0] i;
	integer j;

	initial
		begin
			$display("Starting shift test");

			for (j=0; j<64; j = j + 1)
				begin
					$display("1 << %d == %08h", j, 1<<j);
				end
			
			i = 32'hdeadbeef;
			for (j=0; j<8; j = j + 1)
				begin
					$display("i = %08h", i);
					i = i << 4;
				end

			i = 32'hdeadbeef;
			for (j=0; j<8; j = j + 1)
				begin
					$display("i = %08h", i);
					i = i >> 4;
				end

			big = 128'h0123456789abcdefdeadbeefcafebabe;
	
			$display("big<<4   = %032h", big<<4);
			$display("big<<32  = %032h", big<<32);
			$display("big<<39  = %032h", big<<39);
			$display("big<<64  = %032h", big<<64);
			$display("big<<68  = %032h", big<<68);
			$display("big<<200 = %032h", big<<200);

			$display("big>>4   = %032h", big>>4);
			$display("big>>32  = %032h", big>>32);
			$display("big>>39  = %032h", big>>39);
			$display("big>>64  = %032h", big>>64);
			$display("big>>68  = %032h", big>>68);
			$display("big>>200 = %032h", big>>200);

		end

endmodule
