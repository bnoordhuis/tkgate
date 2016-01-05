module register #(.Dsetup(1), .Dhold(1), .Dck_q(1)) (Q, D, EN, CLR, CK);
  input CK,EN,CLR;
  input ${DQ_RANGE} D;
  output ${DQ_RANGE} Q;
  reg 	 ${DQ_RANGE} Qreg;
 
  specify
      $setup(D,posedge CK &&& (!EN && CLR), Dsetup);
      $hold(posedge CK &&& (!EN && CLR),D, Dhold);
  endspecify

  assign #Dck_q Q = ${invQ}Qreg;

  always @(posedge CK or negedge CLR)
    if (CLR === 1'b0)
      Qreg = ${DQ_BITS}'b0;
    else if (CK === 1'b1 && EN === 1'b0)
      Qreg = D;

endmodule
