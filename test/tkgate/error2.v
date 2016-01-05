module top;
  wire a;

  initial
    begin
      a = 1;
      $display("a=%b",a);
      a = 0;
      $display("a=%b",a);
    end

endmodule
