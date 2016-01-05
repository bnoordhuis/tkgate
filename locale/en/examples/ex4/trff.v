//: version "2.0-b6"
//: property encoding = "iso8859-1"
//: property locale = "en"
//: property prefix = "_GG"
//: property title = "trff.v"
//: property discardChanges = 1

`timescale 1ns/1ns

//: /netlistBegin main
module main;    //: root_module
supply0 w14;    //: /sn:0 {0}(654,415)(654,436)(603,436){1}
//: {2}(599,436)(566,436)(566,415){3}
//: {4}(601,438)(601,455){5}
reg R;    //: {0}(606,353)(535,353){1}
//: {2}(531,353)(503,353){3}
//: {4}(501,351)(501,336){5}
//: {6}(59:499,353)(442,353){7}
//: {8}(533,355)(533,406)(552,406){9}
supply1 w17;    //: /sn:0 {0}(620,315)(620,292){1}
supply1 w12;    //: /sn:0 {0}(625,86)(625,63){1}
supply0 w13;    //: /sn:0 {0}(650,191)(650,213)(608,213){1}
//: {2}(604,213)(571,213)(571,191){3}
//: {4}(606,215)(606,231){5}
reg S;    //: {0}(611,94)(591,94){1}
//: {2}(587,94)(505,94){3}
//: {4}(503,92)(503,81){5}
//: {6}(56:501,94)(436,94){7}
//: {8}(589,96)(589,182)(636,182){9}
wire w6;    //: /sn:0 {0}(625,119)(625,103){1}
wire w16;    //: /sn:0 {0}(620,345)(620,332){1}
wire Q;    //: {0}(50:765,383)(656,383){1}
//: {2}(654,381)(654,273)(532,273)(532,184){3}
//: {4}(534,182)(557,182){5}
//: {6}(532,180)(532,127)(611,127){7}
//: {8}(652,383)(622,383){9}
//: {10}(620,381)(620,362){11}
//: {12}(618,383)(566,383)(566,398){13}
//: {14}(654,385)(654,398){15}
wire _Q;    //: {0}(50:759,159)(694,159){1}
//: {2}(690,159)(652,159){3}
//: {4}(648,159)(627,159){5}
//: {6}(625,157)(625,136){7}
//: {8}(623,159)(571,159)(571,174){9}
//: {10}(650,161)(650,174){11}
//: {12}(692,161)(692,252)(584,252)(584,321){13}
//: {14}(586,323)(606,323){15}
//: {16}(584,325)(584,406)(640,406){17}
//: enddecls

  //: joint g8 (w13) @(606, 213) /anc:1 /w:[ 1 -1 2 4 ]
  //: VDD g4 (w12) @(636,63) /sn:0 /anc:1 /w:[ 1 ]
  _GGNMOS #(2, 1) g13 (.Z(Q), .S(w14), .G(_Q));   //: @(648,406) /sn:0 /anc:1 /w:[ 15 0 17 ]
  _GGPMOS #(2, 1) g3 (.Z(w6), .S(w12), .G(S));   //: @(619,94) /sn:0 /anc:1 /w:[ 1 0 0 ]
  //: joint g34 (S) @(503, 94) /w:[ 3 4 6 -1 ]
  _GGPMOS #(2, 1) g2 (.Z(_Q), .S(w6), .G(Q));   //: @(619,127) /sn:0 /anc:1 /w:[ 7 0 7 ]
  _GGNMOS #(2, 1) g1 (.Z(_Q), .S(w13), .G(S));   //: @(644,182) /sn:0 /anc:1 /w:[ 11 0 9 ]
  //: VDD g16 (w17) @(631,292) /sn:0 /anc:1 /w:[ 1 ]
  //: joint g11 (S) @(589, 94) /anc:1 /w:[ 1 -1 2 8 ]
  //: joint g10 (Q) @(654, 383) /anc:1 /w:[ 1 2 8 14 ]
  //: comment g28 @(26,299) /anc:1
  //: /line:"3) Return the \"S\" switch to the off position"
  //: /line:"and press the space bar 10 to 20 times.  The"
  //: /line:"\"Q\" and \"_Q\" signals will remain constant."
  //: /end
  //: comment g27 @(25,193) /anc:1
  //: /line:"2) Press the space bar 10 to 20 times to"
  //: /line:"advance the simulator.  Note that both the"
  //: /line:"\"Q\" and \"_Q\" signals are in the unknown state."
  //: /line:"Now turn on the \"S\" switch (by clicking on it)"
  //: /line:"and press the space bar another 10 to 20"
  //: /line:"times.  The \"Q\" signal will go high and the"
  //: /line:"\"_Q\" signal will go low."
  //: /line:""
  //: /end
  //: SWITCH g19 (R) @(425,353) /sn:0 /anc:1 /w:[ 7 ] /st:0
  //: LED g32 (Q) @(772,383) /sn:0 /R:3 /w:[ 0 ] /type:0
  //: SWITCH g6 (S) @(419,94) /sn:0 /anc:1 /w:[ 7 ] /st:1
  //: joint g7 (Q) @(620, 383) /anc:1 /w:[ 9 10 12 -1 ]
  //: joint g9 (_Q) @(692, 159) /anc:1 /w:[ 1 -1 2 12 ]
  //: joint g20 (w14) @(601, 436) /anc:1 /w:[ 1 -1 2 4 ]
  _GGPMOS #(2, 1) g15 (.Z(w16), .S(w17), .G(_Q));   //: @(614,323) /sn:0 /anc:1 /w:[ 1 0 15 ]
  //: LED g31 (_Q) @(766,159) /sn:0 /R:3 /w:[ 0 ] /type:0
  //: comment g39 @(20,12) /sn:0
  //: /line:"<a href=\"../index.v\">[BACK]</a>"
  //: /end
  //: comment g25 @(26,42)
  //: /line:"This circuit is a simple flip-flop composed"
  //: /line:"of two CMOS NOR gates implemented at the"
  //: /line:"transistor level."
  //: /end
  //: GROUND g17 (w14) @(601,461) /sn:0 /anc:1 /w:[ 5 ]
  //: comment g29 @(26,355) /anc:1
  //: /line:"4) Now turn the \"R\" switch off by clicking"
  //: /line:"on it and press the space bar 10 to 20 times."
  //: /line:"The \"_Q\" signal will go high and the \"Q\""
  //: /line:"signal will go low."
  //: /end
  _GGPMOS #(2, 1) g14 (.Z(Q), .S(w16), .G(R));   //: @(614,353) /sn:0 /anc:1 /w:[ 11 0 0 ]
  //: GROUND g5 (w13) @(606,237) /sn:0 /anc:1 /w:[ 5 ]
  //: joint g21 (Q) @(532, 182) /anc:1 /w:[ 4 6 -1 3 ]
  //: joint g24 (_Q) @(625, 159) /anc:1 /w:[ 5 6 8 -1 ]
  //: joint g36 (R) @(501, 353) /w:[ 3 4 6 -1 ]
  //: joint g23 (_Q) @(650, 159) /anc:1 /w:[ 3 -1 4 10 ]
  //: comment g26 @(26,101) /anc:1
  //: /line:"1) Begin the simulation by clicking on the"
  //: /line:"\"Play\" button or by selecting \"Begin"
  //: /line:"Simulation\" from the \"Simulate\" menu."
  //: /line:"Set Probes on the \"S\", \"R\", \"Q\" and \"_Q\""
  //: /line:"signals by double-clicking them.  The \"_Q\""
  //: /line:"signal is the \"Q\" with an overline."
  //: /end
  //: joint g22 (R) @(533, 353) /anc:1 /w:[ 1 -1 2 8 ]
  _GGNMOS #(2, 1) g0 (.Z(_Q), .S(w13), .G(Q));   //: @(565,182) /sn:0 /anc:1 /w:[ 9 3 5 ]
  //: LED g35 (R) @(501,329) /sn:0 /w:[ 5 ] /type:0
  //: joint g18 (_Q) @(584, 323) /anc:1 /w:[ 14 13 -1 16 ]
  _GGNMOS #(2, 1) g12 (.Z(Q), .S(w14), .G(R));   //: @(560,406) /sn:0 /anc:1 /w:[ 13 3 9 ]
  //: comment g30 @(23,420) /anc:1
  //: /line:"5) Return the \"R\" switch to the off position"
  //: /line:"and press the space bar 10 to 20 times.  The"
  //: /line:"\"Q\" and \"_Q\" signals will remain constant."
  //: /end
  //: LED g33 (S) @(503,74) /sn:0 /w:[ 5 ] /type:0

endmodule
//: /netlistEnd


`timescale 1ns/1ns


//: /builtinBegin
module _GGNMOS #(.Diz(1), .Dgz(1)) (Z, S, G);
   input  S, G;
   output  Z;

//   specify
//      (G *> Z) = Dgz;
//      (S *> Z) = Diz;
//   endspecify

   nmos #Dgz mos (Z, S, G);

endmodule // nmos_trans
//: /builtinEnd


//: /builtinBegin
module _GGPMOS #(.Diz(1), .Dgz(1)) (Z, S, G);
   input  S, G;
   output  Z;

//   specify
//      (G *> Z) = Dgz;
//      (S *> Z) = Diz;
//   endspecify

   pmos #Dgz pos (Z, S, G);

endmodule // pmos_trans
//: /builtinEnd

