module newdemux #(.Dez(1), .Diz(1)) (I, S, $${NUMOUT:, :Z%});
   input ${I_RANGE} I;
   input ${S_RANGE} S;
   output ${I_RANGE} $${NUMOUT:, :Z%};
   wire ${OUT_RANGE} Zcat;

   specify
$${NUMOUT:\n:      (S *> Z%) = Dsz; } 
$${NUMOUT:\n:      (I *> Z%) = Diz; } 
   endspecify

   assign Zcat = ${invZ} ( ((${NUMOUT}'b1 << I) != 0) ? I : ${I_BITS}'h0;

$${NUMOUT:\n:   assign Z% = Zcat[%];} 
   
endmodule // demux
