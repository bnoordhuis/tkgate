//: beginnetlist top
module top;
  reg [31:0] a,b,c;

  initial
    begin
	fork
	  @(a) $display("got a");
	  @(b) $display("got b");
	  @(c) $display("got c");
	join
	$display("done with fork");
    end

  initial
    begin
	#1 a = 1;
	#1 b = 1;
	#1 c = 1;
    end

endmodule
//:endnetlist top
