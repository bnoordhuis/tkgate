module or #(.Diz(1)) ($${NUMIN:I%, } Z);
   output ${Z_RANGE} Z;
   input  ${I_RANGE} $${NUMIN:, :I%};

     assign #Diz Z = ${invZ}( $${NUMIN: | :I%} );
   
endmodule // or
