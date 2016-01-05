module test;
  reg a,b,e_wire,e_wor,e_wand,e_tri0,e_tri1;
  wire w1;
  wor w2;
  wand w3;
  tri0 w4;
  tri1 w5;

  assign w1 = a;	
  assign w1 = b;	

  assign w2 = a;	
  assign w2 = b;	

  assign w3 = a;	
  assign w3 = b;	

  assign w4 = a;	
  assign w4 = b;	

  assign w5 = a;	
  assign w5 = b;

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
	$display("  WIRE            WOR             WAND            TRI0            TRI1");
	$display("  0 1 x z L H     0 1 x z L H     0 1 x z L H     0 1 x z L H     0 1 x z L H");
	$display(" +-----------    +-----------    +-----------    +-----------    +-----------");
	for (i = 0;i < 6;i = i + 1)
	  begin
	    a = getbit(i);
	
	    $write("%b|",a);
	    for (j = 0;j < 6;j = j + 1)
	      begin
		b = getbit(j);
		#1;
		if (j > 0) $write(" ");
		$write("%b",w1);
	      end

	    $write("   %b|",a);
	    for (j = 0;j < 6;j = j + 1)
	      begin
		b = getbit(j);
		#1;
		if (j > 0) $write(" ");
		$write("%b",w2);
	      end


	    $write("   %b|",a);
	    for (j = 0;j < 6;j = j + 1)
	      begin
		b = getbit(j);
		#1;
		if (j > 0) $write(" ");
		$write("%b",w3);
	      end


	    $write("   %b|",a);
	    for (j = 0;j < 6;j = j + 1)
	      begin
		b = getbit(j);
		#1;
		if (j > 0) $write(" ");
		$write("%b",w4);
	      end


	    $write("   %b|",a);
	    for (j = 0;j < 6;j = j + 1)
	      begin
		b = getbit(j);
		#1;
		if (j > 0) $write(" ");
		$write("%b",w5);
	      end


	    $write("\n");
	  end

      $display("");
      $display("Expect:");
      $display("  WIRE            WOR        	WAND     	TRI0             TRI1       ");
      $display("  0 1 x z L H     0 1 x z L H     0 1 x z L H     0 1 x z L H    0 1 x z L H");
      $display(" +-----------    +-----------    +-----------    +-----------   +-----------");
      $display("0|0 x x 0 0 x   0|0 1 x 0 0 x   0|0 0 0 0 0 0   0|0 x x 0 0 x  0|0 x x 0 0 x");
      $display("1|x 1 x 1 x 1   1|1 1 1 1 1 1   1|0 1 x 1 x 1   1|x 1 x 1 x 1  1|x 1 x 1 x 1");
      $display("x|x x x x x x   x|x 1 x x x x   x|0 x x x x x   x|x x x x x x  x|x x x x x x");
      $display("z|0 1 x z L H   z|0 1 x z L H   z|0 1 x z L H   z|0 1 x 0 0 x  z|0 1 x 1 x 1");
      $display("L|0 x x L L x   L|0 1 x L L x   L|0 x x L L x   L|0 x x 0 0 x  L|0 x x x x x");
      $display("H|x 1 x H x H   H|x 1 x H x H   H|0 1 x H x H   H|x 1 x x x x  H|x 1 x 1 x 1");

    end

endmodule




   




































