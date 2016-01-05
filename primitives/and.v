module and #(.Diz(1)) ($${NUMIN:I%, } Z);
   output ${Z_RANGE} Z;
   input  ${I_RANGE} $${NUMIN:, :I%};
   reg ${Z_RANGE} Z;

   assign #Diz Z = ${invZ}( $${NUMIN: & :I%} );
   
endmodule // and
