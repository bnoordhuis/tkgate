//: version "2.0"
//: property prefix = "_GG"
//: property title = "sim_tut.v"
//: property useExtBars = 0
//: property discardChanges = 1

//: /netlistBegin PAGE1
module PAGE1;    //: root_module
reg B;    //: {0}(50:153,316)(167,316){1}
//: {2}(171,316)(189,316){3}
//: {4}(193,316)(241,316){5}
//: {6}(191,314)(191,253)(236,253){7}
//: {8}(169,314)(169,304){9}
reg A;    //: {0}(236,248)(204,248){1}
//: {2}(202,246)(-86:202,228)(172,228){3}
//: {4}(170,226)(170,215){5}
//: {6}(168,228)(155,228){7}
//: {8}(202,250)(202,311)(241,311){9}
reg Cin;    //: {0}(271,213)(291,213){1}
//: {2}(293,211)(293,202){3}
//: {4}(41:293,215)(293,244){5}
//: {6}(295,246)(333,246){7}
//: {8}(293,248)(293,272){9}
wire w6;    //: /sn:0 {0}(334,314)(262,314){1}
wire w0;    //: /sn:0 {0}(334,309)(291,309)(291,293){1}
wire w1;    //: /sn:0 {0}(333,251)(290,251){1}
//: {2}(286,251)(257,251){3}
//: {4}(288,253)(288,272){5}
wire C;    //: {0}(-21:403,221)(403,312)(355,312){1}
wire S;    //: {0}(354,249)(-39:373,249)(373,221){1}
//: enddecls

  _GGNAND2 #(4) g8 (.I0(w0), .I1(w6), .Z(C));   //: @(345,312) /sn:0 /w:[ 0 0 1 ]
  _GGXOR2 #(8) g4 (.I0(A), .I1(B), .Z(w1));   //: @(247,251) /sn:0 /w:[ 0 7 3 ]
  //: comment g13 @(10,10) /anc:1
  //: /line:"<h3>Combinational Circuit Simulation</h3> <b>(starting the simulator)</b>"
  //: /line:""
  //: /line:"Press the \"<img src=\"simstart.gif\"> <font color=red2>Simulate</font>\" tab above the main canvas to enter simulation mode.  When you first enter simulation"
  //: /line:"mode, the simulator will be paused as indicated by the depressed <img src=sim_pause.gif> button on the simulator tool bar.  Press"
  //: /line:"the <img src=\"sim_go.gif\"> button to begin simulation.  Now try clicking on the switches to change their state and observe how the"
  //: /line:"LEDs change.  When you are done, stop the simulator by pressing <img src=sim_stop.gif> and go on to the next page."
  //: /end
  //: SWITCH g3 (Cin) @(254,213) /sn:0 /w:[ 0 ] /st:1
  _GGNAND2 #(4) g2 (.I0(A), .I1(B), .Z(w6));   //: @(252,314) /sn:0 /w:[ 9 5 1 ]
  //: SWITCH g1 (B) @(136,316) /sn:0 /w:[ 0 ] /st:1
  //: joint g11 (w1) @(288, 251) /w:[ 1 -1 2 4 ]
  //: LED g28 (A) @(170,208) /sn:0 /w:[ 5 ] /type:0
  //: joint g10 (Cin) @(293, 246) /w:[ 6 5 -1 8 ]
  //: joint g27 (Cin) @(293, 213) /w:[ -1 2 1 4 ]
  //: joint g6 (B) @(191, 316) /w:[ 4 6 3 -1 ]
  _GGNAND2 #(4) g9 (.I0(w1), .I1(Cin), .Z(w0));   //: @(291,283) /sn:0 /R:3 /w:[ 5 9 1 ]
  _GGXOR2 #(8) g7 (.I0(Cin), .I1(w1), .Z(S));   //: @(344,249) /sn:0 /w:[ 7 0 0 ]
  //: joint g31 (A) @(170, 228) /w:[ 3 4 6 -1 ]
  //: LED g29 (B) @(169,297) /sn:0 /w:[ 9 ] /type:0
  //: LED g25 (C) @(403,214) /sn:0 /w:[ 0 ] /type:0
  //: joint g5 (A) @(202, 248) /w:[ 1 2 -1 8 ]
  //: LED g26 (Cin) @(293,195) /sn:0 /w:[ 3 ] /type:0
  //: SWITCH g0 (A) @(138,228) /sn:0 /w:[ 7 ] /st:0
  //: comment g12 @(10,410) /sn:0 /R:14 /anc:1
  //: /line:"<tutorial-navigation byfile=1>"
  //: /end
  //: LED g18 (S) @(373,214) /sn:0 /w:[ 1 ] /type:0
  //: joint g30 (B) @(169, 316) /w:[ 2 8 1 -1 ]

endmodule
//: /netlistEnd

//: /builtinBegin
module _GGXOR2 #(.Diz(1)) (I0, I1, Z);
output Z;
input I0;
input I1;

  assign #Diz Z = I0 ^ I1;

endmodule
//: /builtinEnd

//: /builtinBegin
module _GGNAND2 #(.Diz(1)) (I0, I1, Z);
output Z;
input I0;
input I1;

  assign #Diz Z = ~(I0 & I1);

endmodule
//: /builtinEnd
