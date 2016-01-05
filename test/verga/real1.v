module top;
  real n,a,b,c;
  integer i;
  reg [31:0] r;

  initial
    begin
	n = 8'hf;
	$display("The number is %g (should be 15)",n);
	n = 1.1 + 4;
	$display("The number is %g (should be 5.1)",n);


	a = 1.2;
	b = 2.3;
	c = a + b;
	$display("The number is %g (should be 3.5)",c);

	$display("The number is %g (should be 12.65)",(1.2+1.1)*(2.3+3.2));

	a = 1;	
	b = 3;
	$display("The number is %g (should be 0.333333)",a/b);
	$display("The number is %g (should be -1)",-a);
	$display("The number is %g (should be 2)",b-a);

	a = 1.2;
	i = 1;
	$display("Expecting 1, got ",i);
	$display("Expecting 1.2, got ",a);
	r = 1.2;
	$display("Expecting 32'h1, got ",r);
	a = r;
	$display("Expecting 1, got ",a);
    end
endmodule
