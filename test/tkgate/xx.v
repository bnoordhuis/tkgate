//: version "2.0"
//: property prefix = "_GG"
//: property title = "x.v"

//: /netlistBegin main
module main;    //: root_module
wire w0;    //: /sn:0 {0}(155,118)(155,133){1}
wire w3;    //: /sn:0 {0}(165,162)(165,177){1}
wire w1;    //: /sn:0 {0}(175,118)(175,133){1}
wire w2;    //: /sn:0 {0}(127,149)(142,149){1}
//: enddecls

  _GGMUX2 #(8, 8) g0 (.I0(w0), .I1(w1), .S(w2), .Z(w3));   //: @(165,149) /sn:0 /anc:1 /w:[ 1 1 1 0 ] /ss:0 /do:0

endmodule
//: /netlistEnd
