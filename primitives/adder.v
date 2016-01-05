
module adder #(.Dab_s(1), .Dab_co(1), .Dci_s(1), .Dci_co(1)) (A, B, S, CI, CO);
   input  CI;
   output CO;
   input  ${ABS_RANGE} A,B;
   output ${ABS_RANGE} S;
   wire   ${ABS_RANGE} _S;
   wire   _CO;
   
   specify
      (A,B *> S) = Dab_s;
      (A,B *> CO) = Dab_co;
      (CI *> S) = Dci_s;
      (CI *> CO) = Dci_co;
   endspecify

   assign {_CO,_S} = A + B + CI;

   assign CO = ${invCO} _CO;
   assign S = ${invS} _S;

endmodule
