module top;
	reg [127: 0] big;
	reg [31:0] i;
	integer j;

	initial
		begin
			$display("Starting ashift test");

			i = 32'hdeadbeef;
			for (j=0; j<8; j = j + 1)
				begin
					$display("i = %08h", i);
					i = i >>> 4;
				end

			big = 128'h8123456789abcdefdeadbeefcafebabe;
			for (j=0; j<32; j = j + 1)
				begin
					$display("big = %032h", big);
					big = big >>> 4;
				end

			big = 128'h8123456789abcdefdeadbeefcafebabe;
			$display("big>>>4   = %032h", big>>>4);
			$display("big>>>32  = %032h", big>>>32);
			$display("big>>>39  = %032h", big>>>39);
			$display("big>>>64  = %032h", big>>>64);
			$display("big>>>68  = %032h", big>>>68);
			$display("big>>>200 = %032h", big>>>200);


		end

endmodule
