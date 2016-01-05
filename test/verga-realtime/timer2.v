module top;

  time t;

  initial
    begin
	t = $tkg$systime;
	forever
 	   begin
		$display("hello world %t",t);
		t = t + 1000;
		$tkg$waituntil(t);
	   end
    end

endmodule
