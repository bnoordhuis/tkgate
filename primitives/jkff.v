module jkff #(.Dsetup(1), .Dhold(1), .Dck_q(1)) (Q, _Q, J, K, PRE, CLR, CK);
   input  CK,PRE,CLR;
   input  ${JK_RANGE} J, K;
   output ${JK_RANGE} Q,_Q;
   reg 	  ${JK_RANGE} Qreg;
 
   specify
      $setup(J,posedge CK &&& (!PRE && !CLR), Dsetup);
      $setup(K,posedge CK &&& (!PRE && !CLR), Dsetup);
      $hold(posedge CK &&& (!PRE && !CLR),J, Dhold);
      $hold(posedge CK &&& (!PRE && !CLR),K, Dhold);
   endspecify

   assign #Dck_q Q = ${invQ}Qreg;
   assign #Dck_q _Q = ${invNQ}Qreg;

   initial
     if (!PRE)
       Qreg = ${JK_BITS}'b0;
     else if (!CLR)
       Qreg = ~${JK_BITS}'b0;
   
   //
   // Handle preset and clear signals 
   //
   always @ (PRE or CLR)
     if (!PRE)
       Qreg = ${JK_BITS}'b0;
     else if (!CLR)
       Qreg = ~${JK_BITS}'b0;

  always @(posedge CK)
    if (CLR === 1'b1 && PRE === 1'b1)
      Qreg = (J&~Qreg) | (J&~K) | (Qreg&~K);

endmodule
