
module mult #(.Dab_p(1)) (A, B, P);
   input  ${P_RANGE} A,B;
   output ${P_RANGE} P;
   
   assign #Dab_p P = ${invP} (A * B);

endmodule
