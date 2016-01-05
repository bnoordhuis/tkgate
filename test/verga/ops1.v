module top;

  reg [127:0] x,a,b,c;
  reg[31:0] y;

  initial
    begin
	a = 1652763623;
	b = 1287368762;
	c = 2763876432;
	x = a*b*c;
	$display("%d*%d*%d = %d",a,b,c,x);

	x = 32'hffffffff * 32'hffffffff;
	$display("x=%d x=%h",x,x);
	x = 32'hffffffff + 1;
	$display("x=%d x=%h",x,x);

	$display("%d * %d / %d = %d",a,b,c,a*b/c);

	a = 625376212786237651312;
	b = 127834612723233768273;
	$display("%d / %d = %d",a,b,a/b);
	$display("%d %% %d = %d",a,b,a%b);

	a = 625376212786237651312;
	b = 728176123827834612723233768273;
	$display("%d / %d = %d",a,b,a/b);
	$display("%d %% %d = %d",a,b,a%b);


	$display("%d >> 2 = %d",a,a>>2);
	$display("%d << 2 = %d",a,a<<2);
	$display("%d << 33 = %d",a,a<<33);
    end

endmodule
