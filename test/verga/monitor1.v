module top;
  reg [7:0] a,o,x,b,no,nx,na;

  assign a = & b;
  assign o = | b;
  assign x = ^ b;

  assign na = ~& b;
  assign no = ~| b;
  assign nx = ~^ b;

  initial $monitor("%2h:  &:%2h  |:%2h  ^:%2h  ~&:%2h  ~|:%2h  ~^:%2h",b,a,o,x,na,no,nx);

  initial
    begin
	#1 b = 8'h0;
	#1 b = 8'h4;
	#1 b = 8'h7;
	#1 b = 8'hff;
	#1 b = 8'hfz;
	#1 b = 8'h8z;
	#1 b = 8'h0z;
    end

endmodule
