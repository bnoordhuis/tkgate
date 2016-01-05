module top;
  reg [63:0] b;
  reg [7:0] c;
  integer i;

  initial
    begin
      $display("b1=%016h",b);
      b = 64'h0;
      $display("b2=%016h",b);
      b[40 +: 8] = 8'hff;
      $display("b3=%016h",b);
      b = 64'h0;
      b = 64'h0;
      $display("b4=%016h",b);
      b[44 +: 8] = 8'hff;
      $display("b5=%016h",b);
    end

endmodule
