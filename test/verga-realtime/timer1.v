module top;

  always
    begin
	$display("hello world %t",$tkg$systime);
	$tkg$wait(1000);
    end

endmodule
