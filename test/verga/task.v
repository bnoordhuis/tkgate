module top;

  wire [3:0] w1 = 4'h9;
  wire [3:0] w2 = 4'h3;
  wire [7:0] a;
  reg [3:0] r;
  reg [7:0] r2;
  integer i;

  assign a =  1'h1 + foo(8,9);

  initial
    begin
      #0;
      r = 4'b1;
      $display("stating top.  w1=%h",w1);
      mytask(w2);
      $display("after call.");
      fooby(3,7,r2);
      $display("r2=%d.",r2);
    end

  initial
    for (i = 0;i < 50;i = i + 10)
      #1 $display("in top (i=%d, w1='h%h, a='h%h, r='h%h).",i,w1,a,r);

  automatic function [7:0] foo(input [7:0] a,input [7:0] b);
    begin
      $display("in foo(%d, %d)",a,b);
      foo = a + b;
    end
  endfunction

  automatic task mytask(input [7:0] a);
    integer i;

    begin
      for (i = 0;i < 5;i = i + 1)
         #1 $display("in mytask (i=%d, w1='h%h, a='h%h).",i,w1,a);
    end
  endtask

  task fooby(input [7:0] a,input [7:0] b,output [7:0] c);
    integer i;

    begin
	$display("start of fooby");
	c = a*b; 
	$display("end of fooby");
    end
  endtask

endmodule
