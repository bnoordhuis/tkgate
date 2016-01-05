module top;

  integer i;
  reg [31:0] m[0:1024];

  initial
    begin
      $display("starting Fibonacci");

      m[0] = 1;
      m[1] = 1;
      for (i = 2;i < 20;i = i + 1)
	begin
	  m[i] = m[i-1] + m[i-2];
	end
      for (i = 0;i < 20;i = i + 1)
	begin
	  $display("m[%d] = %d",i,m[i]);
	end
    end

endmodule
