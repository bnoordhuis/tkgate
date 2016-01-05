module top;
  integer i,a;

  initial
    begin
	$monitor("%d: $monitor a=%d",$time,a);
	for (i = 0;i < 20;i = i + 1)
	  begin
	    if ( (i & 1) == 0)
	      a <= i;

	    if (i == 8) $monitoroff;
	    if (i == 14) $monitoron;
	    if (i == 16) $monitor("%d: $monitor(2) 3*a=%d",$time,3*a);

	    $display("%d: $display 2*a=%d",$time,2*a);
	    $strobe("%d: $strobe 2*a=%d",$time,2*a);
	    #1;
	  end
    end
endmodule
