module top;
   reg ck;

   always
     begin
	#20 ck = ~ck;
      end


   always @(ck) $display("ck=%b",ck);

   initial
	begin
	   ck = 1'b0;
	   #500 $finish;
	end

   foo f1;

endmodule

module foo;
   initial
	$display("hello from foo");

endmodule
