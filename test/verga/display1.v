module top;
  reg [7:0] a,b,c,d;

  initial
    begin
	a = 8'h7a;
	b = 0;
        c = 1;
        d = a[4:2];
	$display("%h",a[c +: 4]);
	$display("%h",a[4:2]);
	$display("%h",d);
    end

endmodule
