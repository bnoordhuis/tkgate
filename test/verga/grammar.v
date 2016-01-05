module bar #(.delay1(4)) (a,b,c,z);

  assign #delay1 z = a & b & c;

endmodule

module var #(delay1 = 4) (a,b,c,z);

  assign #delay1 z = a & b & c;

endmodule

module foo(a,b,c,z);
input [3:0] a,b,c;
output z;
reg [7:0] m[0:65536];

  assign #1 a = b & c | d & e; 

  foo #(.a(1), .b(3)) f1[3:0] (a,b), g1(.i(d), .o(e));

  always @(a or b)
    begin:foo
	#3 b = c + 2;
	#4 a = 1 + 2;
	a = #6 1 + 2;
	z <= #6 1 + 2;
	if (a > b)
	  begin
	    a = b + c;
	    v = x + c;
	  end
	else
	  h = a + k;

	case (s)
	  8'h7z: u = r + 4;
	  8'h?9: 
	    begin
	      u = r + 9;
	      r = u - 1;
            end
	  default: u = r + 1;
	endcase
    end  

  initial
    begin
      for (i = 0;i < 10;i = i + 1)
         x = x + i;

      while (e < a) e = w;

      @(posedge CLK) w = e;

      repeat (20) e = w;

      repeat (20) @ (posedge CLK);
//      fork
//        @ (posedge A);
//        @ (posedge B);
//      join

    end

endmodule

module \module ;
endmodule