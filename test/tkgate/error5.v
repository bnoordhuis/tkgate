//: version "2.0"
//: property prefix = "_GG"
//: property title = "error5.v"

//: /netlistBegin main
module main;    //: root_module
wire w0;    //: /sn:0 {0}(159,147)(174,147){1}
wire w3;    //: /sn:0 {0}(279,245)(291,245){1}
wire w1;    //: /sn:0 {0}(159,152)(174,152){1}
wire w2;    //: /sn:0 {0}(195,150)(210,150){1}
//: enddecls

  foo g2 ();   //: @(195, 289) /sz:(40, 40) /sn:0 /p:[ ]
  foo g1 (.A(w3));   //: @(292, 201) /sz:(100, 118) /sn:0 /p:[ Li0>1 ]
  _GGAND2 #(6) g0 (.I0(w0), .I1(w1), .Z(w2));   //: @(185,150) /sn:0 /w:[ 1 1 0 ]

endmodule
//: /netlistEnd

//: /netlistBegin foo
module foo(A);
//: interface  /sz:(40, 40) /bd:[ ]
input A;    //: /sn:0 /dp:1 {0}(244,220)(198,220){1}
wire bufout;    //: /sn:0 /dp:1 {0}(260,220)(275,220){1}
wire w0;    //: /sn:0 {0}(233,349)(248,349){1}
wire w1;    //: /sn:0 {0}(264,349)(279,349){1}
//: enddecls

  _GGBUF #(4) g2 (.I(w0), .Z(w1));   //: @(254,349) /sn:0 /w:[ 1 0 ]
  _GGBUF #(4) g1 (.I(A), .Z(bufout));   //: @(250,220) /sn:0 /w:[ 0 0 ]
  //: IN g0 (A) @(196,220) /sn:0 /w:[ 1 ]

endmodule
//: /netlistEnd

//: /builtinBegin
module _GGBUF #(.Diz(1)) (I, Z);
input I;
output Z;
  assign #Diz Z = I;

endmodule
//: /builtinEnd

//: /builtinBegin
module _GGAND2 #(.Diz(1)) (I0, I1, Z);
output Z;
input I0;
input I1;

  assign #Diz Z = I0 & I1;

endmodule
//: /builtinEnd
