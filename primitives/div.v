
module div #(.Dab_q(1), .Dab_r(1)) (A, B, Q, R);
   input  ${ABQR_RANGE} A,B;
   output ${ABQR_RANGE} Q,R;
   
   assign #Dab_q Q = ${invQ} (A / B);
   assign #Dab_r R = ${invR} (A % B);

endmodule   // div
