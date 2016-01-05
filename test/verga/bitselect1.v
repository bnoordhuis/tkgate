module top;
  reg [63:0] a;
  reg [64:0] b;
  reg [62:0] c;
  reg [7:0] q;
  integer i;

  initial
    begin
      a = 64'h123456789abcdef1;
      for (i = 0;i <= 56;i = i + 4)
	begin
	  q = a[i +: 8];
	  $display("q=%h",q);
	end

      q = 8'hff;
      for (i = 0;i <= 56;i = i + 4)
	begin
	  a = 0;
	  a[i +: 8] = q;
	  $display("i=%2d  a=%016h",i,a);
	end
      $display("  ----   ");

      b = 64'h123456789abcdef1;
      for (i = 0;i <= 56;i = i + 4)
	begin
	  q = b[i +: 8];
	  $display("q=%h",q);
	end

      q = 8'hff;
      for (i = 0;i <= 56;i = i + 4)
	begin
	  b = 0;
	  b[i +: 8] = q;
	  $display("i=%2d  b=%016h",i,b);
	end

      $display("  ----   ");

      c = 63'h123456789abcdef1;
      for (i = 0;i < 56;i = i + 4)
	begin
	  q = c[i +: 8];
	  $display("q=%h",q);
	end

      q = 8'hff;
      for (i = 0;i < 56;i = i + 4)
	begin
	  c = 0;
	  c[i +: 8] = q;
	  $display("i=%2d  c=%016h",i,c);
	end
    end

endmodule
