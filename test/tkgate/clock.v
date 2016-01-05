//: version "2.0"
//: property prefix = "_GG"

//: /netlistBegin main
module main;    //: root_module
wire w0;    //: /sn:0 {0}(270,191)(285,191){1}
//: enddecls

  _GGCLOCK_P100_0_50 g0 (.Z(w0));   //: @(257,191) /sn:0 /w:[ 0 ] /omega:100 /phi:0 /duty:50

endmodule
//: /netlistEnd

//: /builtinBegin
module _GGCLOCK_P100_0_50 (Z);
output Z;
reg Z;

initial #0
  begin 
    forever
      begin
        Z = 1'b0;
        #50;
        Z = 1'b1;
        #50;
      end
   end

endmodule
//: /builtinEnd
