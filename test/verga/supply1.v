module top;
  supply0 gnd;
  reg a;
  wire z;

  AND2 g1(z, a, !gnd);

  initial
    begin
	$monitor("%d: z=%b a=%b",$time,z,a);
	#2 a = 1'b1;
    end

endmodule

module AND2(Z,A,B);
  output Z;
  input A,B;

  assign #1 Z = A & B;
endmodule
