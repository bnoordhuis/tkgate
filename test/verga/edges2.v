//
// Non-blocking assignment test.
//
module top;
  reg x;
  supply1 vcc;
  supply0 gnd;

  always @(posedge x) $display("posedge x=%d",x);
  always @(negedge x) $display("negedge x=%d",x);

  initial
    begin
	$display("x=%d vcc=%d gnd=%d",x,vcc,gnd);
	vcc = 0;
	$display("x=%d vcc=%d gnd=%d",x,vcc,gnd);
	#1 x = vcc;
	#1 x = gnd;
	#1 x = vcc;
	#1 x = gnd;
	#1 x = gnd;
	#1 $display("done");
    end

endmodule

