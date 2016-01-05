module demux #(.Dez(1), .Dfz(1), .Dsz(1)) (F, S, E, $${NUMOUT:, :Z%});
   input ${F_RANGE} F;
   input ${S_RANGE} S;
   input E;
   output ${F_RANGE} $${NUMOUT:, :Z%};
   wire ${F_RANGE} X;

   specify
$${NUMOUT:\n:      (E *> Z%) = Dez; } 
$${NUMOUT:\n:      (F *> Z%) = Dfz; } 
$${NUMOUT:\n:      (S *> Z%) = Dsz; } 
   endspecify

   assign X = E ? F : ${F_BITS}'h0;
   
$${NUMOUT:\n:    assign Z% = (S == ${S_BITS}'d%) ? X : ${F_BITS}'h0;}
   
endmodule // demux
