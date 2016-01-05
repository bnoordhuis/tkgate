module top;
  reg c2;
  reg s1, a1, b1, c1;
  reg s0, a0, b0, c0;
  reg x;
  integer i;

  assign s0 = a0 ^ b0 ^ c0;
  assign s1 = a1 ^ b1 ^ c1;
  assign c1 = a0 & b0 | a0 & c0 | b0 & c0;
  assign c2 = a1 & b1 | a1 & c1 | b1 & c1;

  assign x = a1 ? a0 : 1'bz;
  assign x = b1 ? b0 : 1'bz;

  initial
    begin
	$display("Starting SUM test");
  	        c0 = 1;
	a1 = 1; a0 = 0;
	b1 = 0; b0 = 1;
	#1 ;
	$display("s0=%b ",s0);
	$display("s1=%b ",s1);
	$display("c2=%b ",c2);
	$display("sum: %b = %b + %b + %b",{c2,s1,s0},{a1,a0},{b1,b0},c0);

	$display("");
	$display("Starting tri-state");
        for (i = 0;i < 16;i = i + 1)
          begin
	    a0 = (i & 8'h1) != 0;
	    a1 = (i & 8'h2) != 0;
	    b0 = (i & 8'h4) != 0;
	    b1 = (i & 8'h8) != 0;
            #1 ;
            $display("%b = buf(%b)if(%b) ++  buf(%b)if(%b)",x,a0,a1,b0,b1);
          end
    end

endmodule
