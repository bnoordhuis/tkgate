module top;

  M64 m64;
  M48 m48;
  S s;
  REP rep;

endmodule

module M64;

  reg [31:0] a,b,c,d;
  reg [63:0] x;

  initial
    begin
	a = 32'h12345678;
	b = 32'habcd0123;
	x = {a,b};
	{c,d} = x;
	$display("a=%h  b=%h  x=%h  c=%h  d=%h",a,b,x,c,d);
    end
endmodule

module M48;

  reg [23:0] a,b,c,d;
  reg [47:0] x;

  initial
    begin
	a = 24'h345678;
	b = 24'hcd0123;
	x = {a,b};
	{c,d} = x;
	$display("a=%h  b=%h  x=%h  c=%h  d=%h",a,b,x,c,d);
    end
endmodule

module S;
  initial
    begin
	$display("%s",{"this ","is ","a test"," of some strings"," being concatenated."});
    end
endmodule

module REP;
  reg [159:0] s;
  wire [159:0] w;

  assign w = {4{"XYZ:"}};

  initial
    begin
	$display("%s",{5{"hello world! "}});
	$display("%s",{10{"abcd "}});
	s = {4{"ABC:"}};
	$display("%s",s);
	#1 $display("%s",w);
    end
endmodule
