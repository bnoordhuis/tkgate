module top;
  integer i,j;
  reg a,c;
  wire w,x,y,z;

  function getbit;
  input [31:0] i;

    case (i)
      0: getbit = 1'b0;
      1: getbit = 1'b1;
      2: getbit = 1'bx;
      3: getbit = 1'bz;
      4: getbit = 1'bl;
      5: getbit = 1'bh;
    endcase
  endfunction

  initial
    $monitor("%4d: a=%b c=%b : pmos=%b nmos=%b bufif1=%b bufif0=%b",$time,a,c,w,x,y,z);

  initial
    for (i = 0;i < 6;i = i + 1)
      begin
	a = getbit(i);
	for (j = 0;j < 6;j = j + 1)
	  begin
	    c = getbit(j);
	    #10;
	  end
      end
 
  pmos    b(w,a,c);
  nmos    b(x,a,c);
  bufif1  b(y,a,c);
  bufif0  b(z,a,c);

endmodule
