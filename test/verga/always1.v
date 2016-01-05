module top;
  reg[31:0] r,b;
  integer i,j;

  initial
    for (i = 0;i < 10;i = i + 1)
      begin
	#2 r = i;
	$display("r=%d in <%m>",r);
      end 

  initial
    for (j = 0;j < 10;j = j + 1)
      begin
        if (j < 7)
          #3 b = j;
        else
          #5 b = 2*j;
	$display("b=%d",b);
      end 

  always @(r or b)
    begin
      $display("@%d change in r=%d or b=%d",$time,r,b);
    end 

endmodule

