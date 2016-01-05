module mux #(.Dsz(1), .Diz(1)) ($${NUMIN:I%, }S, Z);
   input ${IZ_RANGE} $${NUMIN:, :I%};
   input ${S_RANGE} S;
   output ${IZ_RANGE} Z;
   reg 	  ${IZ_RANGE} Z;

   always
     begin
	case (S)
$${NUMIN:\n:	  ${S_BITS}'d%: Z <= #Dsz ${invZ} I%;}
	  default: Z <= #Dsz ${invZ} ${IZ_BITS}'hx;
	endcase // case(S)
	@(S or $${NUMIN: or :I%});
     end
      
endmodule // mux
