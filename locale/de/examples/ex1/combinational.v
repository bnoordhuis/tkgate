//: version "2.0-b6"
//: property encoding = "utf-8"
//: property locale = "de"
//: property prefix = "_GG"
//: property title = "combinational.v"
//: property discardChanges = 1
//: require "tty"
//: require "coke"

`timescale 1ns/1ns

//: /netlistBegin main
module main;    //: root_module
reg A0;    //: {0}(173,94)(163,94){1}
//: {2}(67:159,94)(95,94){3}
//: {4}(161,96)(161,154)(173,154){5}
reg w0;    //: /sn:0 {0}(239,92)(219,92){1}
//: {2}(217,90)(217,50){3}
//: {4}(217,94)(217,117){5}
reg A2;    //: {0}(165,415)(155,415){1}
//: {2}(60:151,415)(96,415){3}
//: {4}(153,417)(153,475)(165,475){5}
reg B2;    //: {0}(30:100,480)(142,480){1}
//: {2}(146,480)(165,480){3}
//: {4}(144,478)(144,420)(165,420){5}
reg B1;    //: {0}(50:98,317)(142,317){1}
//: {2}(146,317)(169,317){3}
//: {4}(144,315)(144,257)(169,257){5}
reg A1;    //: {0}(169,252)(159,252){1}
//: {2}(61:155,252)(96,252){3}
//: {4}(157,254)(157,312)(169,312){5}
reg B0;    //: {0}(50:93,159)(144,159){1}
//: {2}(148,159)(173,159){3}
//: {4}(146,157)(146,99)(173,99){5}
wire w6;    //: /sn:0 {0}(215,171)(215,138){1}
wire S1;    //: {0}(50:303,253)(257,253){1}
wire w14;    //: /sn:0 {0}(211,329)(211,296){1}
wire w19;    //: /sn:0 {0}(231,413)(211,413){1}
//: {2}(209,411)(209,350){3}
//: {4}(209,415)(209,438){5}
wire w4;    //: /sn:0 {0}(239,97)(214,97){1}
//: {2}(210,97)(194,97){3}
//: {4}(212,99)(212,117){5}
wire w3;    //: /sn:0 {0}(210,171)(210,157)(194,157){1}
wire C3;    //: {0}(23:303,533)(205,533)(205,513){1}
wire w21;    //: /sn:0 {0}(202,492)(202,478)(186,478){1}
wire S0;    //: {0}(50:303,95)(260,95){1}
wire w18;    //: /sn:0 {0}(231,418)(206,418){1}
//: {2}(202,418)(186,418){3}
//: {4}(204,420)(204,438){5}
wire w22;    //: /sn:0 {0}(207,492)(207,459){1}
wire w11;    //: /sn:0 {0}(236,250)(215,250){1}
//: {2}(213,248)(213,192){3}
//: {4}(213,252)(213,275){5}
wire w10;    //: /sn:0 {0}(236,255)(210,255){1}
//: {2}(206,255)(190,255){3}
//: {4}(208,257)(208,275){5}
wire w13;    //: /sn:0 {0}(206,329)(206,315)(190,315){1}
wire S2;    //: {0}(50:303,416)(252,416){1}
//: enddecls

  _GGNAND2 #(4) g8 (.I0(w4), .I1(w0), .Z(w6));   //: @(215,128) /sn:0 /R:3 /w:[ 5 5 1 ] /eb:0
  //: SWITCH g4 (B0) @(76,159) /sn:0 /w:[ 0 ] /st:1
  //: LED g37 (S2) @(310,416) /sn:0 /R:3 /w:[ 0 ] /type:0
  //: comment g34 @(351,22)
  //: /line:"Ein einfacher 3-Bit-Addierer."
  //: /line:""
  //: /line:"Starte den Simulator durch Drücken des \"Abspielen\""
  //: /line:"Knopfes aus der Knopfleiste. Setze dann Tastköpfe"
  //: /line:"durch Doppelklick auf die Leitungen S0, S1, S2 and C3"
  //: /line:"(und beliebige zusätzliche Leitungen, die getastet"
  //: /line:"werden sollen).  Drücke wieder den \"Abspielen\" Knopf"
  //: /line:"um in den kontinuierlichen Simulationsmodus einzutreten."
  //: /line:"Immer, wenn sich ein Schalterwert ändert (durch Klicken)"
  //: /line:"simuliert der Simulator so lange, bis sich alle Werte"
  //: /line:"stabilisiert haben."
  //: /line:"Beende die Simulation durch Drücken des \"stop\""
  //: /line:"Knopfes auf der Knopfleiste."
  //: /end
  _GGXOR2 #(8) g13 (.I0(A1), .I1(B1), .Z(w10));   //: @(180,255) /sn:0 /w:[ 0 5 3 ] /eb:0
  //: SWITCH g3 (w0) @(217,37) /sn:0 /R:3 /w:[ 3 ] /st:0
  _GGXOR2 #(8) g2 (.I0(w0), .I1(w4), .Z(S0));   //: @(250,95) /sn:0 /w:[ 0 0 1 ] /eb:0
  _GGXOR2 #(8) g1 (.I0(A0), .I1(B0), .Z(w4));   //: @(184,97) /sn:0 /w:[ 0 5 3 ] /eb:0
  _GGNAND2 #(4) g16 (.I0(A1), .I1(B1), .Z(w13));   //: @(180,315) /sn:0 /w:[ 5 3 1 ] /eb:0
  _GGNAND2 #(4) g11 (.I0(w3), .I1(w6), .Z(w11));   //: @(213,182) /sn:0 /R:3 /w:[ 0 0 3 ] /eb:0
  //: joint g28 (A2) @(153, 415) /w:[ 1 -1 2 4 ]
  //: joint g10 (w4) @(212, 97) /w:[ 1 -1 2 4 ]
  //: joint g32 (w18) @(204, 418) /w:[ 1 -1 2 4 ]
  _GGNAND2 #(4) g27 (.I0(A2), .I1(B2), .Z(w21));   //: @(176,478) /sn:0 /w:[ 5 3 1 ] /eb:0
  _GGNAND2 #(4) g19 (.I0(w10), .I1(w11), .Z(w14));   //: @(211,286) /sn:0 /R:3 /w:[ 5 5 1 ] /eb:0
  //: LED g38 (C3) @(310,533) /sn:0 /R:3 /w:[ 0 ] /type:0
  //: joint g6 (A0) @(161, 94) /w:[ 1 -1 2 4 ]
  //: joint g9 (w0) @(217, 92) /w:[ 1 2 -1 4 ]
  //: joint g7 (B0) @(146, 159) /w:[ 2 4 1 -1 ]
  //: joint g31 (w19) @(209, 413) /w:[ 1 2 -1 4 ]
  //: joint g20 (w11) @(213, 250) /w:[ 1 2 -1 4 ]
  //: SWITCH g15 (B1) @(81,317) /sn:0 /w:[ 0 ] /st:0
  //: comment g39 @(20,12) /sn:0
  //: /line:"<a href=\"../index.v\">[ZURÜCK]</a>"
  //: /end
  //: joint g29 (B2) @(144, 480) /w:[ 2 4 1 -1 ]
  _GGXOR2 #(8) g25 (.I0(w19), .I1(w18), .Z(S2));   //: @(242,416) /sn:0 /w:[ 0 0 1 ] /eb:0
  //: joint g17 (A1) @(157, 252) /w:[ 1 -1 2 4 ]
  _GGXOR2 #(8) g14 (.I0(w11), .I1(w10), .Z(S1));   //: @(247,253) /sn:0 /w:[ 0 0 1 ] /eb:0
  _GGNAND2 #(4) g5 (.I0(A0), .I1(B0), .Z(w3));   //: @(184,157) /sn:0 /w:[ 5 3 1 ] /eb:0
  //: LED g36 (S1) @(310,253) /sn:0 /R:3 /w:[ 0 ] /type:0
  _GGXOR2 #(8) g24 (.I0(A2), .I1(B2), .Z(w18));   //: @(176,418) /sn:0 /w:[ 0 5 3 ] /eb:0
  //: joint g21 (w10) @(208, 255) /w:[ 1 -1 2 4 ]
  //: SWITCH g23 (A2) @(79,415) /sn:0 /w:[ 3 ] /st:0
  //: LED g35 (S0) @(310,95) /sn:0 /R:3 /w:[ 0 ] /type:0
  //: SWITCH g26 (B2) @(83,480) /sn:0 /w:[ 0 ] /st:0
  _GGNAND2 #(4) g22 (.I0(w13), .I1(w14), .Z(w19));   //: @(209,340) /sn:0 /R:3 /w:[ 0 0 3 ] /eb:0
  //: SWITCH g0 (A0) @(78,94) /sn:0 /w:[ 3 ] /st:0
  //: joint g18 (B1) @(144, 317) /w:[ 2 4 1 -1 ]
  //: SWITCH g12 (A1) @(79,252) /sn:0 /w:[ 3 ] /st:1
  _GGNAND2 #(4) g33 (.I0(w21), .I1(w22), .Z(C3));   //: @(205,503) /sn:0 /R:3 /w:[ 0 0 1 ] /eb:0
  _GGNAND2 #(4) g30 (.I0(w18), .I1(w19), .Z(w22));   //: @(207,449) /sn:0 /R:3 /w:[ 5 5 1 ] /eb:0

endmodule
//: /netlistEnd


`timescale 1ns/1ns


//: /builtinBegin
module _GGXOR2 #(.Diz(1)) (I0, I1,  Z);
   output  Z;
   input   I0, I1;

     assign #Diz Z = ( I0 ^ I1 );
   
endmodule // xor

//: /builtinEnd


//: /builtinBegin
module _GGNAND2 #(.Diz(1)) (I0, I1,  Z);
   output  Z;
   input   I0, I1;
   reg  Z;

   assign #Diz Z = ~( I0 & I1 );
   
endmodule // and
//: /builtinEnd

