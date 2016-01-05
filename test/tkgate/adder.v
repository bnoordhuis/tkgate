//: version "2.0"
//: property prefix = "_GG"
//: property title = "Welcome to TkGate Page"
//: property discardChanges = 1

//: /netlistBegin main
module main;    //: root_module
reg [7:0] w19;    //: /sn:0 {0}(#:330,184)(330,158)(281,158)(281,145){1}
reg [7:0] w0;    //: /sn:0 {0}(362,184)(362,143)(461,143)(#:461,132){1}
supply0 w11;    //: /sn:0 {0}(386,220)(386,198)(370,198){1}
wire w24;    //: /sn:0 {0}(322,198)(312,198){1}
wire [7:0] w22;    //: /sn:0 {0}(#:321,239)(321,225)(346,225)(346,213){1}
//: enddecls

  //: GROUND g43 (w11) @(386,226) /sn:0 /w:[ 0 ]
  //: DIP g42 (w19) @(281,135) /sn:0 /w:[ 1 ] /st:248
  //: DIP g0 (w0) @(461,122) /sn:0 /w:[ 1 ] /st:0
  _GGADD8 #(68, 70, 62, 64) g45 (.A(w19), .B(w0), .S(w22), .CI(w11), .CO(w24));   //: @(346,200) /sn:0 /w:[ 0 0 1 1 0 ]

endmodule
//: /netlistEnd

//: /builtinBegin
module _GGADD8 #(.Dab_s(1), .Dab_co(1), .Dci_s(1), .Dci_co(1)) (A, B, S, CI, CO);
input CI;
output CO;
input [7:0] A,B;
output [7:0] S;

  specify
    (A,B *> S) = Dab_s;
    (A,B *> CO) = Dab_co;
    (CI *> S) = Dci_s;
    (CI *> CO) = Dci_co;
  endspecify

  assign {CO,S} = A + B + CI;

endmodule
//: /builtinEnd
