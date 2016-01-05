module top;
  wire [7:0] Q;
  reg [7:0] D;
  reg EN,CLR,CK;

  _GGREG8 g1(Q,D,EN,CLR,CK);
 
  initial
    begin
	$monitor("%2d: Q=%h  D=%h  EN=%h  CLR=%h",$time,Q,D,EN,CLR);

	CLR = 1'b1;
	EN = 1'b0;
	CK = 1'b0;
	D = 7'h42;
        #3;
	#1 CK = 1'b1;
	#2 CK = 1'b0;
	D = 7'h99;
	EN = 1'b1;
	#1 CK = 1'b1;
	#2 CK = 1'b0;
    end

endmodule


module _GGREG8 #(Dsetup=1, Dhold=1, Dck_q=1) (Q, D, EN, CLR, CK);
input CK,EN,CLR;
input [7:0] D;
output [7:0] Q;
reg [7:0] Qreg;

  assign #Dck_q Q = Qreg;

  always @(posedge CK or negedge CLR)
    if (!CLR)
      Qreg = 0;
    else if (CK && ~EN)
      Qreg = D;

endmodule
