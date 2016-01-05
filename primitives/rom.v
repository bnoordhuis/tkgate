module rom #(.Dout(1), .Dread(1)) (A, D, OE);
   input  OE;
   input  ${A_RANGE} A;
   output ${D_RANGE} D;
   reg 	  ${D_RANGE} m${MEM_RANGE};
   wire   ${D_RANGE} X;

   assign #Dread X = m[A];
   assign #Dout D = (~OE) ? ${invD}X : ${D_BITS}'bz;

endmodule
