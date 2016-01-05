module top;

  parameter N = 32;
  reg [N-1:0] m[0:1024];
  reg [N-1:0] r;

  initial
    begin
      $display("#### starting memory bit-range");
      m[0] = 32'h12345678;
      r = 32'h12345678;
      $display("#### m[0][7:0]=%h",m[0][7:0]);
      $display("#### m[0][15:8]=%h",m[0][15:8]);

      $display("#### r[7:0]=%h",r[7:0]);
      $display("#### r[15:8]=%h",r[15:8]);

      m[0][23:16] = 16'haa;
      $display("#### m[0]=%h",m[0]);

      m[0] = 8'haa;
      $display("#### m[0]=%h",m[0]);
    end

    always @(m)
      begin
	$display("m event");
      end

endmodule
