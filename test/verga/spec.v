module top;
  integer i;
  reg a,b,c;
  wire x,y,z;

  initial
    $monitor("%4d: a=%b b=%b c=%b - x=%b y=%b z=%b",$time,a,b,c,x,y,z);

  initial
    for (i = 0;i < 8;i = i + 1)
      begin
	{a,b,c} = i;
	#50;
//	$finish();
      end

  foo f(a,b,c,x,y,z);

endmodule

module foo(a,b,c,x,y,z);
input a,b,c;
output x,y,z;
wire q,r;

  specify
    (a,b *> x) = 12;
    (c *> x) = 8;
    (a *> y) = 11;
    (c *> y) = 16;
    (b *> z) = 23;
    (c *> z) = 18;
  endspecify

//  and (r, q, c);
//  xor (x, a, b, r);

  assign r = q & c;
  assign x = a ^ b ^ r;
  assign y = a & c;
  assign z = c & b;
  assign q = a & b;

endmodule
