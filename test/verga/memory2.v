module top;

  parameter N = 32;
  reg [N-1:0] m[0:1024];
  integer i;

  initial
    begin
	$readmemh("test.mem");
	for (i = 0;i < 20;i = i + 1)
	  $display("m[%d] = %h",i,m[i]);
    end

    always @(m)
      begin
	$display("m event");
      end

endmodule
