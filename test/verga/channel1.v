module top;

  integer i,j;
  reg DSR;

  initial
    for (i = 0;i < 10;i = i + 1)
      begin
	$display("writing %d",i);
	$tkg$send("xyz",i);
	#1 ;
      end

  always
    forever
      begin
	j = $tkg$recv("xyz");
	$display("read %d",j);
      end

endmodule

