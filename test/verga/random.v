module top;

  initial
    begin
	$random(100);
	repeat (20)
	  $display($random);
    end

endmodule
