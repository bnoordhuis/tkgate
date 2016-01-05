
module top;
  reg [7:0] a,b;
  reg [4:0] c,d;
  reg [10*8-1:0] x,y;
  integer i;

  initial
    begin
	a = 8'h42;

	{c,d} = a;
	$display("c=%h  d=%h",c,d);

	b[7:4] = 4'h6;
	b[3:0] = 4'h8;
	$display("b=%h",b);

	{a[3:0],a[7:4]} = a & 8'hff;
	$display("a=%h",a);

	x = "ABCDEFGHIJ";
    	$display("x=%s  x=%h",x,x);
 	for (i = 0;i < 10;i = i+1)
	  $display("x[%d:%d]=%s",8*i+7,8*i,x[8*i +: 8]);

 	for (i = 0;i < 10;i = i+1)
	  x[8*i +: 8] = (i+1)*(i+1);
	$display("x=%h",x);

 	for (i = 0;i < 10;i = i+1)
 	  begin	
	    y[8*i +: 8] = x[8*(9-i) +: 8];
	  end
	$display("y=%020h",y);

	// 
	// i should be read only on entering the loop so updates to i in
	// the body should not affect the number of iterations.
	// 
	i = 5;
	repeat (i)
	  begin
	    i = 2*(i - 1);
	    $display("hello i = %d",i);
	  end
	$display("done hello");
    end

endmodule
