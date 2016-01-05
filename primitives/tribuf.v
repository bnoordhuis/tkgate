module tribuf #(.Dez(1), .Diz(1)) (Z, I, E);
   input ${IZ_RANGE} I;
   input E;
   output ${IZ_RANGE} Z;

   specify
      (E *> Z) = Dez;
      (I *> Z) = Diz;
   endspecify

   assign Z = E ? (${invZ}I) : ${IZ_BITS}'bz;
   
endmodule // buf
