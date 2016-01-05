module decoder #(.Dez(1), .Diz(1)) (I, E, $${NUMOUT:, :Z%});
   input ${I_RANGE} I;
   input E;
   output $${NUMOUT:, :Z%};
   wire ${OUT_RANGE} Zcat;

   specify
$${NUMOUT:\n:      (E *> Z%) = Dez; } 
$${NUMOUT:\n:      (I *> Z%) = Diz; } 
   endspecify

   assign Zcat = ${invZ} ({${NUMOUT}{E}} & (${NUMOUT}'b1 << I));

$${NUMOUT:\n:   assign Z% = Zcat[%];} 
   
endmodule // decoder
