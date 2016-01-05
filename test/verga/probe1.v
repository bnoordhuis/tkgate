module top;
  integer i,a;

  initial
    begin
	$tkg$probe("bob",a,i);

	for (i = 0;i < 20;i = i + 1)
	  begin
	    if ( (i & 1) == 0)
	      a <= i;

	    #1;
	  end
    end
endmodule
