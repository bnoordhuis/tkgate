module top;
  reg a1, b1;
  reg a0, b0, c0;
  wire s1, s0, c1, c2;
  integer i;

//  assign s1 = a1 ^ b1 ^ c1;
//  assign s0 = a0 ^ b0 ^ c0;

  assign {s1,s0} = {a1,a0} ^ {b1,b0} ^ {c1,c0};

  assign c1 = a0 & b0 | a0 & c0 | b0 & c0;
  assign c2 = a1 & b1 | a1 & c1 | b1 & c1;


  initial
    begin
	$display("Starting SUM test");
        for (i = 0;i < 32;i = i + 1)
	  begin
	    a0 = (i & 8'h1) != 0;
	    a1 = (i & 8'h2) != 0;
	    b0 = (i & 8'h4) != 0;
	    b1 = (i & 8'h8) != 0;
	    c0 = (i & 8'h10) != 0;
	    #1 $display("sum: %3b = %2b + %2b + %2b       (%d=%d+%d+%d)",
			{c2,s1,s0},{a1,a0},{b1,b0},c0,
			{c2,s1,s0},{a1,a0},{b1,b0},c0);
	  end
    end
endmodule
