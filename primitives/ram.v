
module ram #(.Dout(1),		// Output time
	     .Dread(1),		// Read time
	     .Dwrite(1),	// Write time
	     .Daddr_setup(1),	// Address setup time
	     .Daddr_hold(1),	// Address hold time
	     .Ddata_hold(1))	// Data hold time
	(A, D, WE, OE, CS);
	    
   input OE, WE, CS;
   input ${A_RANGE} A;
   inout ${D_RANGE} D;
   reg 	 ${D_RANGE} m${MEM_RANGE};
   wire  ${D_RANGE} X;
   wire  doRead, doWrite;

   specify
      $setup(A, posedge doWrite, Daddr_setup);
      $hold(negedge doWrite, A, Daddr_hold);
      $hold(negedge doWrite, D, Ddata_hold);
      $width(posedge doWrite, Dwrite);
   endspecify

   assign doRead = (!CS && !OE);
   assign doWrite = (!CS && !WE);

   //
   // Value read from addressed memory
   //
   assign #Dread X = m[A];

   //
   // If chip-select and output-enable are asserted, output value from memory
   //
   assign #Dout D = doRead ? X : ${D_BITS}'bz;

   //
   // Look for conditions on which to write to memory
   //
   always @(doWrite or D or A)
     if (doWrite)
       m[A] <= #Dwrite D;

   always @(D)
     if (doWrite)
       $error("`RAMDCHG %m[%M]");

   always @(A)
     if (doWrite)
       $error("`RAMACHG %m[%M]");
   

endmodule // ram
