module top;
  reg[31:0] r,a,q;
  integer i;

  initial
      begin
	a = 0;
//
// Using this statment should result in r=0
//
	q <= 8;
	r = #2 a;

//
// Using this statment should result in r=2
//
//	#2 r = a;
	$display("r=%d",r);
      end 

  initial
      begin
	#1 a = 2;
      end 


endmodule

