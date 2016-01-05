module top;
  integer i;
  reg a,b;
  wire x;

  initial
    $monitor("%4d: a=%b b=%b - x=%b",$time,a,b,x);

  initial
    for (i = 0;i < 4;i = i + 1)
      begin
	{a,b} = i;
	#50;
//	$finish();
      end

  foo f(a,b,x);

endmodule

module foo(a,b,x);
input a,b;
output x;

  specify
    if (a) (a *> x) = 10;
    if (!a) (a *> x) = 21;
    (b *> x) = 12;
  endspecify

  assign x = a ^ b;

endmodule
