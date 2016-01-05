module top;
  wire [3:0] s;
  wire [4:0] c;
  reg [3:0] a,b;
  reg ci;
  

  adder a1 (s[0], c[1], a[0], b[0], ci);
  adder a2 (s[1], c[2], a[1], b[1], c[1]);
  adder a3 (s[2], c[3], a[2], b[2], c[2]);
  adder a4 (s[3], c[4], a[3], b[3], c[3]);

  initial
    begin
	$monitor("%d: %h = %h + %h + %h",$time,{c[4],s},a,b,ci);

	#2 a = 4'b0; b = 4'b0; ci = 1'b0;
	#2 a = 4'h2; b = 4'h7; ci = 1'b0;
	#2 a = 4'h2; b = 4'h7; ci = 1'b1;
	#2 a = 4'h9; b = 4'h3; ci = 1'b1;
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
