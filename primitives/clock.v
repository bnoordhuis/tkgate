module clock (Z);
   output Z;
   reg 	  Z = ${invZ} 1'b${INITIAL};

   initial #${PHASE}
     if (${INITIAL} == 0)
       forever
	 begin
	    Z = ${invZ} 1'b1;
	    #${ON_TIME};
	    Z = ${invZ} 1'b0;
	    #${OFF_TIME};
	 end
     else
       forever
	 begin
	    Z = ${invZ} 1'b0;
	    #${OFF_TIME};
	    Z = ${invZ} 1'b1;
	    #${ON_TIME};
	 end
   
endmodule // clock
