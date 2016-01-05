module top;
  reg [31:0] b;

  initial
    begin
	$display("n=%h",'h12345678abcdef1);
	b = 'h12345678abcdef1;
	$display("b1=%h",b);
	b[19:12] = 64'h12345678abcdef1;
	$display("b1=%h",b);
    end

endmodule
