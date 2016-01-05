module top;
  wire [3:0] s;
  wire co;
  reg [3:0] a,b;
  reg ci;

  adder a1 (s, co, a, b, ci);

  initial
    begin
	$monitor("%d: %h = %h + %h + %h",$time,{co,s},a,b,ci);

	#2 a = 4'b0; b = 4'b0; ci = 1'b0;
	#2 a = 4'h2; b = 4'h7; ci = 1'b0;
	#2 a = 4'h2; b = 4'h7; ci = 1'b1;
	#2 a = 4'h9; b = 4'h3; ci = 1'b1;
	#2 a = 4'hf; b = 4'h3; ci = 1'b0;
    end

endmodule

module adder(s,co,a,b,ci);
  output [3:0] s;
  output co;
  input [3:0] a,b;
  input ci;
  wire [2:0] c;
  wire [3:0] x1,w1,w2,w3;

  xor g1[3:0] (x1,a,b);
  xor g1[3:0] (s,x1,{c[2:0],ci});

  and g1[3:0] (w1,a,b);
  and g1[3:0] (w2,a,{c[2:0],ci});
  and g1[3:0] (w3,b,{c[2:0],ci});

  or  g1[3:0] ({co,c}, w1, w2, w3);

endmodule
