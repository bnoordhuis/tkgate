module nmos_trans #(.Diz(1), .Dgz(1)) (Z, S, G);
   input ${SGZ_RANGE} S, G;
   output ${SGZ_RANGE} Z;

//   specify
//      (G *> Z) = Dgz;
//      (S *> Z) = Diz;
//   endspecify

   nmos #Dgz mos${SGZ_RANGE} (Z, ${invZ}S, G);

endmodule // nmos_trans
