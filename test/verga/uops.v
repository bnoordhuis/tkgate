module top;
  reg a,b;
  reg [31:0] x;
  reg [31:0] r;

 
  initial
    begin
      x = ~0;
      a = x;
      b = ~x;
      #1;
      $display("a=%h  b=%h",a,b);
      if (a) $display("a is true.");
      if (~a) $display("~a is true.");
      if (!a) $display("!a is true.");
      if (b) $display("b is true.");
      if (~b) $display("~b is true.");
      if (!b) $display("!b is true.");

      r = 32'hz;
      $display("r='h%h   r='o%o",r,r);
    end

endmodule

