module top;
  reg s1,s0;
  reg a1,a0,b1,b0,c2,c1,c0;

  assign s0 = a0 ^ b0 ^ c0;
  assign s1 = a1 ^ b1 ^ c1;
  assign c1 = a0 & b0 | a0 & c0 | b0 & c0;
  assign c2 = a1 & b1 | a1 & c1 | b1 & c1;

  initial
    begin
	a0 = 0; a1 = 0; b0 = 0; b1 = 0; c0 = 0;
 	#1 $display("sum: %b = %b + %b + %b",{c2,s1,s0},{a1,a0},{b1,b0},c0);
    end


endmodule
