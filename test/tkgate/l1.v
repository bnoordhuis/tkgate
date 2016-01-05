//: version "2.0"
//: property prefix = "_GG"
//: property title = "l1.v"

//: /netlistBegin main
module main;    //: root_module
wire w0;    //: {0}#[45](161,187)(263,187){1}
wire w1;    //: /sn:0 {0}#(248,192)(263,192){1}
wire MyWire;    //: {0}#[55](284,190)(451,190){1}
//: enddecls

  _GGAND2 #(6) g0 (.I0(w0), .I1(w1), .Z(MyWire));   //: @(274,190) /sn:0 /w:[ 1 1 0 ]

endmodule
//: /netlistEnd

//: /builtinBegin
module _GGAND2 #(.Diz(1)) (I0, I1, Z);
output Z;
input I0;
input I1;

  assign #Diz Z = I0 & I1;

endmodule
//: /builtinEnd
