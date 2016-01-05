module test;
  reg [35:0] a;
  trireg [35:0] w1 = 35'bz;
  trireg w2;
  reg b;

  assign w1 = a;	
  assign w2 = b;

  integer i,j;

  function getbit;
  input [31:0] i;

    case (i)
      0: getbit = 1'b0;
      1: getbit = 1'b1;
      2: getbit = 1'bx;
      3: getbit = 1'bz;
      4: getbit = 1'bl;
      5: getbit = 1'bh;
    endcase
  endfunction

  initial
    begin
	//
	// Preset all trireg bits
	//
	for (i = 0;i < 6;i = i + 1)
	  for (j = 0;j < 6;j = j + 1)
	    begin
	      #1 a[i*6+j] = getbit(i);
	      #1 a[i*6+j] = 1'bz;
	    end

	$display("  TRIREG     ");
	$display("  0 1 x z L H");
	$display(" +-----------");
	for (i = 0;i < 3;i = i + 1)
	  begin
	    $write("%b|",getbit(i));
	    for (j = 0;j < 6;j = j + 1)
	      begin
		#1 a[i*6+j] = getbit(j);
		#1;
		if (j > 0) $write(" ");
		$write("%b",w1[i*6+j]);
	      end

	    $write("\n");
	  end

      $display("");
      $display("Expect:");
      $display("  TRIREG     ");
      $display("  0 1 x z L H");
      $display(" +-----------");
      $display("0|0 1 x 0 0 x");
      $display("1|0 1 x 1 x 1");
      $display("x|0 1 x x x x");
/*
      $display("z|0 1 x z L H");
      $display("L|0 1 x L L x");
      $display("H|0 1 x H x H");
*/

      $display("");

      #1 $display("w2=%b (expect x)",w2);
      b = 1'b0;
      #1 $display("w2=%b (expect 0)",w2);
      b = 1'bz;
      #1 $display("w2=%b (expect 0)",w2);
      b = 1'b1;
      #1 $display("w2=%b (expect 1)",w2);
      b = 1'bz;
      #1 $display("w2=%b (expect 1)",w2);

    end

endmodule
