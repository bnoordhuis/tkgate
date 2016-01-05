//
// Non-blocking assignment test.
//
module top;
  reg[31:0] a,b,c;
  integer i;

  initial
      begin
	#1 a = 1;
	#1 b = 2;
	$display("%d: a=%d b=%d",$time,a,b);
	a <= b;
	b <= a;
	#1 $display("%d: a=%d b=%d",$time,a,b);
	a <= #2 b;
	b <= #3 a;
	#1 $display("%d: a=%d b=%d",$time,a,b);
	#1 $display("%d: a=%d b=%d",$time,a,b);
	#1 $display("%d: a=%d b=%d",$time,a,b);
	#1 $display("%d: a=%d b=%d",$time,a,b);
	$display("");
        c = 1;
        c <= @(a) 2;
        a <= #2 3;
	#1 $display("%d: a=%d c=%d",$time,a,c);
	@(c) $display("%d: a=%d c=%d",$time,a,c);
	#1 $display("%d: a=%d c=%d",$time,a,c);
      end 

endmodule

