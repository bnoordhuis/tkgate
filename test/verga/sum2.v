module top;
  parameter n = 2;

  reg [n-1:0] a, b;
  reg ci;
  wire [n-1:0] s;
  wire [n:0] c;
  integer i;

  assign c[0] = ci;
  assign s = a ^ b ^ c[n-1:0];
  assign c[n:1] = a & b | a & c[n-1:0] | b & c[n-1:0];

  initial
    begin
	$display("Starting SUM test");
        for (i = 0;i < 32;i = i + 1)
	  begin
	    a[0] = (i & 8'h1) != 0;
	    a[1] = (i & 8'h2) != 0;
	    b[0] = (i & 8'h4) != 0;
	    b[1] = (i & 8'h8) != 0;
	    ci = (i & 8'h10) != 0;
	    #100 $display("sum: %3b = %2b + %2b + %2b       (%d=%d+%d+%d)",{c[2],s},a,b,c[0],{c[2],s},a,b,c[0]);
	  end
    end
endmodule
