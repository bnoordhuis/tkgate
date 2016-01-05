module top;
  event e;

  initial
    begin
	#0;
	-> e;
	-> e;
	#10;
	-> e;
	-> e;
    end

  always @(e)
    $display("@%d: triggered on e",$time);


endmodule
