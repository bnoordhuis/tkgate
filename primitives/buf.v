module buf #(.Diz(1)) (I, Z);
   input ${IZ_RANGE} I;
   output ${IZ_RANGE} Z;

   assign #Diz Z = ${invZ}I;
   
endmodule // buf
