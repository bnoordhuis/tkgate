//: version "2.0"
//: property prefix = "_GG"
//: property title = "error.v"

//: /netlistBegin main
module main;    //: root_module
reg w4;    //: /sn:0 {0}(111,321)(191,321)(191,320)(201,320){1}
reg w3;    //: /sn:0 {0}(112,291)(201,291){1}
reg w0;    //: /sn:0 {0}(113,195)(201,195){1}
reg w1;    //: /sn:0 {0}(113,225)(201,225){1}
reg w2;    //: /sn:0 {0}(112,257)(202,257){1}
reg w5;    //: /sn:0 {0}(112,353)(191,353)(191,352)(201,352){1}
//: enddecls

  //: SWITCH g4 (w2) @(95,257) /sn:0 /w:[ 0 ] /st:0
  bar g3 ();   //: @(-68, 129) /sz:(40, 40) /sn:0 /p:[ ]
  //: SWITCH g2 (w1) @(96,225) /sn:0 /w:[ 0 ] /st:0
  //: SWITCH g1 (w0) @(96,195) /sn:0 /w:[ 0 ] /st:0
  //: SWITCH g6 (w4) @(94,321) /sn:0 /w:[ 0 ] /st:0
  //: SWITCH g7 (w5) @(95,353) /sn:0 /w:[ 0 ] /st:0
  //: SWITCH g5 (w3) @(95,291) /sn:0 /w:[ 0 ] /st:0
  foo g0 (.I4(w4), .I3(w3), .I3(w2), .I2(w1), .I1(w0), .I5(w5));   //: @(202, 177) /sz:(70, 207) /sn:0 /p:[ Li0>1 Li1>1 Li2>1 Li3>1 Li4>1 Lt0=1 ]

endmodule
//: /netlistEnd

//: /netlistBegin foo
module foo(I5, I4, I3, I9);
//: interface  /sz:(40, 40) /bd:[ ]
input [1:0] I4;    //: /sn:0 {0}(182,240)(197,240){1}
inout [2:0] I5;    //: /sn:0 /dp:1 {0}(198,294)(213,294){1}
input I9;    //: /sn:0 {0}(202,131)(217,131){1}
input I3;    //: /sn:0 {0}(187,178)(202,178){1}
//: enddecls

  //: INOUT g3 (I5) @(192,294) /sn:0 /w:[ 0 ]
  //: IN g2 (I4) @(180,240) /sn:0 /w:[ 0 ]
  //: IN g1 (I3) @(185,178) /sn:0 /w:[ 0 ]
  //: IN g0 (I9) @(200,131) /sn:0 /w:[ 0 ]

endmodule
//: /netlistEnd

//: /hdlBegin bar
//: interface  /sz:(40, 40) /bd:[ ]
//: enddecls
module bar;
  wire a;
  wire b;
  wire a;
  
  assign h = a;
  
  foo g1(.I1(a), .I2(b));

  initial
    begin
      a = b + 1;
      x = y;
    end

endmodule
//: /hdlEnd
