module top;
   wire w1,w2;
   reg p,q;

   foo f1 (.z(w1),.b(p),.a(q));
   foo f2 (.z(w2),.b(p),.a(q));

   initial $monitor("w1=%b  w2=%b  p=%b  q=%b  f1.v=%b  f2.v=%b",w1,w2,p,q,top.f1.v,top.f2.v);

   initial $stop(7);

   initial
     begin   
	#1 p = 1'b0; q = 1'b0;
	#1 p = 1'b0; q = 1'b1;
	#1 p = 1'b1; q = 1'b0;
	#1 p = 1'b1; q = 1'b1;
	$display("changeing f1.v");
	top.f1.v = 1'b1;
	#1 p = 1'b0; q = 1'b0;
	#1 p = 1'b0; q = 1'b1;
	#1 p = 1'b1; q = 1'b0;
	#1 p = 1'b1; q = 1'b1;
	#1;
     end   
endmodule

module foo(z,a,b);
output z;
input a;
input b;
reg v;

  initial v = 0;

  assign z = a ^ b ^ v;

endmodule
