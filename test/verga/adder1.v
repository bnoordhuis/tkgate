module top;
  wire s,co;
  reg a,b,ci;

  adder a1(s,co,a,b,ci);

  initial
    begin
	$monitor("%d: {%h,%h} = %h + %h + %h",$time,co,s,a,b,ci);

	#2 a = 1'b0; b = 1'b0; ci = 1'b0;
	#2 a = 1'b0; b = 1'b0; ci = 1'b1;
	#2 a = 1'b0; b = 1'b1; ci = 1'b0;
	#2 a = 1'b0; b = 1'b1; ci = 1'b1;
	#2 a = 1'b1; b = 1'b0; ci = 1'b0;
	#2 a = 1'b1; b = 1'b0; ci = 1'b1;
	#2 a = 1'b1; b = 1'b1; ci = 1'b0;
	#2 a = 1'b1; b = 1'b1; ci = 1'b1;
    end

endmodule

module adder(s,co,a,b,ci);
  output s,co;
  input a,b,ci;
  wire x1,w1,w2,w3;

  xor (x1,a,b);
  xor (s,x1,ci);

  and (w1,a,b);
  and (w2,a,ci);
  and (w3,b,ci);

  or (co, w1, w2, w3);

endmodule
