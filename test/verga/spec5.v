module top;
  integer i;
  reg  a,b;
  wire x,y;

  initial
    $monitor("%4d: a=%d b=%d - x=%d y=%d",$time,a,b,x,y);

  initial
    for (i = 0;i < 4;i = i + 1)
      begin
	{a,b} = i;
	#100;
      end

  foo f(a,b,x,y);

endmodule

module foo #(.p1(10), .p2(21)) (a,b,x,y);
input a,b;
output x,y;

  specify
    if (a) (a *> x) = p1;
    if (!a) (a *> x) = p2;
    (b *> x) = 12;
    (a, b *> y) = 17;
  endspecify

  assign {x,y} = a + b;

endmodule
