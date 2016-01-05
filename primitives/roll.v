module roll #(.Dsz(1), .Diz(1)) (I, Z , S);
   input ${IZ_RANGE} I;
   input ${S_RANGE} S;
   output ${IZ_RANGE} Z;

   specify
      (I *> Z) = Diz;
      (S *> Z) = Dsz;
   endspecify

   assign Z = ${invZ} ((I << (S % ${IZ_BITS})) | (I >> ((${IZ_BITS} - S) % ${IZ_BITS})));
   
endmodule // lshift
