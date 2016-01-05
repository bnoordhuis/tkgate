module top;
  reg a1, b1;
  reg a0, b0;
  reg x;
  integer i;


  assign x = a1 ? a0 : 1'bz;
  assign x = b1 ? b0 : 1'bz;

  initial
    begin
	$display("Starting tri-state");
        for (i = 0;i < 16;i = i + 1)
          begin
	    a0 = (i & 8'h1) != 0;
	    a1 = (i & 8'h2) != 0;
	    b0 = (i & 8'h4) != 0;
	    b1 = (i & 8'h8) != 0;
            #1 $display("%b = buf(%b)if(%b) ++  buf(%b)if(%b)",x,a0,a1,b0,b1);
          end
    end
endmodule
