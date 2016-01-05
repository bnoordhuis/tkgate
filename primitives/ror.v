module ror #(.Diz(1)) (I0, Z);
   output ${Z_RANGE} Z;
   input  ${I_RANGE} I0;

   assign #Diz Z = ${invZ} | I0;
   
endmodule // ror
