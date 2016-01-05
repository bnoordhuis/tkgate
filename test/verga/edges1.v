//
// Non-blocking assignment test.
//
module top;
  reg[31:0] a,b,c;
  reg x;
  integer i;

  always @(*)
    begin
      a = b + c;
    end 

  always @(posedge x)
    $display("%d: posedge x=%b",$time,x);

  always @(negedge x)
    $display("%d: negedge x=%b",$time,x);

  always @(a)
    $display("a = %d",a);

  initial
    begin
	b = 0; c = 0;
	#1 b = 1;
	#1 b = 2;
	#1 c = 2;
	#1 c = 2;
	#1 c = 2;
	$display("x=%b",x);
	#1 x = 1;
	#1 x = 0;
	#1 x = 1;
	#1 x = 0;
	#1 $display("done");
    end

endmodule

