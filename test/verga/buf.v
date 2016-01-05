module top;
  reg a;
  wire x,y,z,i,j,k;

  initial
    $monitor("%4d: a=%b : x=%b  y=%b  z=%b : i=%b j=%b k=%b",$time,a,x,y,z,i,j,k);

  initial
    begin
	#10 a = 1;
	#10 a = 0;
	#10 a = 1;
	#10 a = 0;
    end
 
  buf #7 b(x,y,z,a);
  not #5 n(i,j,k,a);

endmodule
