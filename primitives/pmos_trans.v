module pmos_trans #(.Diz(1), .Dgz(1)) (Z, S, G);
   input ${SGZ_RANGE} S, G;
   output ${SGZ_RANGE} Z;

//   specify
//      (G *> Z) = Dgz;
//      (S *> Z) = Diz;
//   endspecify

   pmos #Dgz pos${SGZ_RANGE} (Z, ${invZ}S, G);

endmodule // pmos_trans
