module top;
   wire w1,w2;
   reg p,q,r;

   reg [3:0] m,n;
   wire [1:0] s,v;
   wire [7:0] l;

   foo f1 (.z(w1),.b(w2),.a(r));
   foo f2 (w2,p,q);
   bar b1 ({s,v},m,n);
   bar b2 (l[7:4],m,n);

   initial
     begin   
	p = 1'b0; q = 1'b0; r= 1'b0;
	#1 $display("%b = %b ^ %b ^ %b",w1,p,q,r);

	p = 1'b0; q = 1'b1; r= 1'b0;
	#1 $display("%b = %b ^ %b ^ %b",w1,p,q,r);

	p = 1'b1; q = 1'b0; r= 1'b0;
	#1 $display("%b = %b ^ %b ^ %b",w1,p,q,r);

	p = 1'b1; q = 1'b1; r= 1'b0;
	#1 $display("%b = %b ^ %b ^ %b",w1,p,q,r);

	p = 1'b0; q = 1'b0; r= 1'b1;
	#1 $display("%b = %b ^ %b ^ %b",w1,p,q,r);

	p = 1'b0; q = 1'b1; r= 1'b1;
	#1 $display("%b = %b ^ %b ^ %b",w1,p,q,r);

	p = 1'b1; q = 1'b0; r= 1'b1;
	#1 $display("%b = %b ^ %b ^ %b",w1,p,q,r);

	p = 1'b1; q = 1'b1; r= 1'b1;
	#1 $display("%b = %b ^ %b ^ %b",w1,p,q,r);

	m = 3; n = 8;
	#1 $display("%d = %d + %d   l=%h",{s,v},m,n,l);
     end   
endmodule

module foo(z,a,b);
output z;
input a;
input b;

  assign z = a ^ b;

endmodule

module bar(z,a,b);
output [3:0] z;
input [3:0] a;
input [3:0] b;

  assign z = a + b;

endmodule
