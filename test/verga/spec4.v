module top;
  integer i;
  reg ci = 1'b0;
  reg [1:0] a,b;
  wire [1:0] s;
  wire co;

  initial
    $monitor("%4d: a=%d b=%d ci=%d - s=%d co=%d",$time,a,b,ci,s,co);

  initial
    for (i = 0;i < 16;i = i + 1)
      begin
	{a,b} = i;
	#100;
      end

  _GGADD8 f(a,b,s,ci,co);

endmodule

module _GGADD8 (A, B, S, CI, CO);
input CI;
output CO;
input [7:0] A,B;
output [7:0] S;

  specify
    (A,B *> S) = 1;
    (A,B *> CO) = 1;
    (CI *> S) = 1;
    (CI *> CO) = 1;
  endspecify

  assign {CO,S} = A + B + CI;

endmodule
