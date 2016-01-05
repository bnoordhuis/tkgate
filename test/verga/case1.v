//: beginnetlist top
module top;
  integer i,q;

  initial
    begin
	q = 1 + 1;
	for (i = 0;i < 6;i = i + 1)
	  case (i)
		8'h1: $display("one");
		4'h2: $display("two");
		(q+1): $display("three");
		4: $display("four");
		default: $display("default");
	  endcase
    end

endmodule
//:endnetlist top
