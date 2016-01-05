`timescale 1ns/10ps

module top;
  foo f1;
  bar b1;

  initial
    #5.5 $display("%t (%d): top",$time,$time);

endmodule

`timescale 100ps/10ps

module foo;

  initial
    #5 $display("%t (%d): foo",$time, $time);

endmodule

`timescale 10ps/10ps

module bar;

  initial
    #5 $display("%t (%d): bar",$time, $time);

endmodule
