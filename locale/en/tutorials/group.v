//: version "2.0-b6"
//: property encoding = "iso8859-1"
//: property locale = "en"
//: property prefix = "_GG"
//: property title = "edit2_tut.v"
//: property useExtBars = 0
//: property discardChanges = 1

`timescale 1ns/1ns

//: /netlistBegin PAGE1
module PAGE1;    //: root_module
//: enddecls

  //: comment g1 @(140,67) /sn:0 /anc:1
  //: /line:"<img src=bigcircuit.gif>"
  //: /end
  //: comment g5 @(10,10) /anc:1
  //: /line:"<h3>Group Editing Features</h3>"
  //: /line:""
  //: /line:"In this chapter, you will learn about how to do editing operations on groups of gates."
  //: /end
  //: comment g0 @(10,310) /sn:0 /R:14 /anc:1
  //: /line:"<tutorial-navigation>"
  //: /end

endmodule
//: /netlistEnd

//: /netlistBegin PAGE3
module PAGE3();
//: interface  /sz:(40, 40) /bd:[ ]
reg c0;    //: {0}(18:183,147)(289,147)(289,189){1}
//: {2}(291,191)(314,191){3}
//: {4}(289,193)(289,210){5}
reg a0;    //: {0}(40:183,183)(227,183){1}
//: {2}(231,183)(239,183){3}
//: {4}(229,185)(229,239)(246,239){5}
reg b0;    //: {0}(50:183,218)(221,218){1}
//: {2}(223,216)(223,188)(239,188){3}
//: {4}(223,220)(223,244)(246,244){5}
wire w7;    //: /sn:0 {0}(267,242)(282,242)(282,254){1}
wire s0;    //: {0}(36:335,189)(365,189){1}
wire w4;    //: /sn:0 {0}(287,231)(287,254){1}
wire c1;    //: {0}(99:285,275)(285,294){1}
wire w9;    //: /sn:0 {0}(314,186)(286,186){1}
//: {2}(282,186)(260,186){3}
//: {4}(284,188)(284,210){5}
//: enddecls

  //: LED g37 (s0) @(372,189) /sn:0 /R:3 /w:[ 1 ] /type:0
  _GGNAND2 #(4) g1 (.I0(w7), .I1(w4), .Z(c1));   //: @(285,265) /sn:0 /R:3 /w:[ 1 1 0 ]
  _GGNAND2 #(4) g28 (.I0(a0), .I1(b0), .Z(w7));   //: @(257,242) /sn:0 /w:[ 5 5 0 ]
  //: SWITCH g27 (c0) @(166,147) /sn:0 /w:[ 0 ] /st:0
  //: joint g32 (w9) @(284, 186) /w:[ 1 -1 2 4 ]
  _GGNAND2 #(4) g31 (.I0(w9), .I1(c0), .Z(w4));   //: @(287,221) /sn:0 /R:3 /w:[ 5 5 0 ]
  //: SWITCH g25 (a0) @(166,183) /sn:0 /w:[ 0 ] /st:0
  //: joint g29 (a0) @(229, 183) /w:[ 2 -1 1 4 ]
  //: comment g5 @(10,10) /anc:1
  //: /line:"<h3>Group Editing Features</h3> <b>(incremental selection)</b>"
  //: /line:""
  //: /line:"You can add or remove gates from the current selection by holding the <font color=red2>Control</font> key while"
  //: /line:"you click on them.  Use the selection box to select the gates below then click on a few"
  //: /line:"of the gates with the <font color=red2>Control</font> key held down.  Release the <font color=red2>Control</font> key and then try moving"
  //: /line:"the selection with only some of the gates selected."
  //: /end
  _GGXOR2 #(8) g24 (.I0(w9), .I1(c0), .Z(s0));   //: @(325,189) /sn:0 /w:[ 0 3 0 ]
  _GGXOR2 #(8) g23 (.I0(a0), .I1(b0), .Z(w9));   //: @(250,186) /sn:0 /w:[ 3 3 3 ]
  //: SWITCH g26 (b0) @(166,218) /sn:0 /w:[ 0 ] /st:0
  //: comment g0 @(10,310) /sn:0 /R:14 /anc:1
  //: /line:"<tutorial-navigation>"
  //: /end
  //: joint g30 (b0) @(223, 218) /w:[ -1 2 1 4 ]
  //: joint g33 (c0) @(289, 191) /w:[ 2 1 -1 4 ]

endmodule
//: /netlistEnd

//: /netlistBegin PAGE5
module PAGE5();
//: interface  /sz:(40, 40) /bd:[ ]
reg c0;    //: {0}(18:167,153)(273,153)(273,195){1}
//: {2}(275,197)(298,197){3}
//: {4}(273,199)(273,216){5}
reg a0;    //: {0}(40:167,189)(189,189)(189,189)(211,189){1}
//: {2}(215,189)(223,189){3}
//: {4}(213,191)(213,245)(230,245){5}
reg b0;    //: {0}(50:167,224)(186,224)(186,224)(205,224){1}
//: {2}(207,222)(207,194)(223,194){3}
//: {4}(207,226)(207,250)(230,250){5}
wire w7;    //: /sn:0 {0}(251,248)(266,248)(266,260){1}
wire s0;    //: {0}(36:319,195)(349,195){1}
wire w4;    //: /sn:0 {0}(271,237)(271,260){1}
wire c1;    //: {0}(99:269,281)(269,300){1}
wire w9;    //: /sn:0 {0}(298,192)(270,192){1}
//: {2}(266,192)(244,192){3}
//: {4}(268,194)(268,216){5}
//: enddecls

  //: LED g37 (s0) @(356,195) /sn:0 /R:3 /w:[ 1 ] /type:0
  //: comment g2 @(570,113) /sn:0 /anc:1
  //: /line:"<font color=green4>Hint: If a selection contains any"
  //: /line:"anchored gates, the entire selection"
  //: /line:"will act as if it were anchored.</font>"
  //: /end
  _GGNAND2 #(4) g1 (.I0(w7), .I1(w4), .Z(c1));   //: @(269,271) /sn:0 /R:3 /w:[ 1 1 0 ]
  _GGNAND2 #(4) g28 (.I0(a0), .I1(b0), .Z(w7));   //: @(241,248) /sn:0 /w:[ 5 5 0 ]
  //: SWITCH g27 (c0) @(150,153) /sn:0 /w:[ 0 ] /st:0
  //: joint g32 (w9) @(268, 192) /w:[ 1 -1 2 4 ]
  _GGNAND2 #(4) g31 (.I0(w9), .I1(c0), .Z(w4));   //: @(271,227) /sn:0 /R:3 /w:[ 5 5 0 ]
  //: SWITCH g25 (a0) @(150,189) /sn:0 /w:[ 0 ] /st:0
  //: joint g29 (a0) @(213, 189) /w:[ 2 -1 1 4 ]
  //: comment g5 @(10,10) /anc:1
  //: /line:"<h3>Group Editing Features</h3> <b>(rotation and anchoring)</b>"
  //: /line:""
  //: /line:"You can rotate (with <img src=edit_rotate.gif bgcolor=gray> and <img src=edit_brotate.gif bgcolor=gray>) and anchor (with <img src=anchor.gif bgcolor=gray> and <img src=unanchor.gif bgcolor=gray>) a group of gates in the same"
  //: /line:"way as you did for single gates in the <a href=gates.v>Editing Gates</a> chapter.  Try it on the circuit below."
  //: /end
  _GGXOR2 #(8) g24 (.I0(w9), .I1(c0), .Z(s0));   //: @(309,195) /sn:0 /w:[ 0 3 0 ]
  _GGXOR2 #(8) g23 (.I0(a0), .I1(b0), .Z(w9));   //: @(234,192) /sn:0 /w:[ 3 3 3 ]
  //: comment g0 @(10,310) /sn:0 /R:14 /anc:1
  //: /line:"<tutorial-navigation>"
  //: /end
  //: SWITCH g26 (b0) @(150,224) /sn:0 /w:[ 0 ] /st:0
  //: joint g30 (b0) @(207, 224) /w:[ -1 2 1 4 ]
  //: joint g33 (c0) @(273, 197) /w:[ 2 1 -1 4 ]

endmodule
//: /netlistEnd

//: /netlistBegin PAGE4
module PAGE4();
//: interface  /sz:(40, 40) /bd:[ ]
reg c0;    //: {0}(18:174,160)(280,160)(280,202){1}
//: {2}(282,204)(305,204){3}
//: {4}(280,206)(280,223){5}
reg a0;    //: {0}(40:174,196)(196,196)(196,196)(218,196){1}
//: {2}(222,196)(230,196){3}
//: {4}(220,198)(220,252)(237,252){5}
reg b0;    //: {0}(50:174,231)(193,231)(193,231)(212,231){1}
//: {2}(214,229)(214,201)(230,201){3}
//: {4}(214,233)(214,257)(237,257){5}
wire w7;    //: /sn:0 {0}(258,255)(273,255)(273,267){1}
wire s0;    //: {0}(36:326,202)(356,202){1}
wire w4;    //: /sn:0 {0}(278,244)(278,267){1}
wire c1;    //: {0}(99:276,288)(276,307){1}
wire w9;    //: /sn:0 {0}(305,199)(277,199){1}
//: {2}(273,199)(251,199){3}
//: {4}(275,201)(275,223){5}
//: enddecls

  //: LED g37 (s0) @(363,202) /sn:0 /R:3 /w:[ 1 ] /type:0
  //: comment g2 @(548,117) /sn:0 /anc:1
  //: /line:"<font color=green4>Hint: Hitting the <font color=red2>Delete</font> key will"
  //: /line:"delete all of the selected gates"
  //: /line:"without copying it to the clipboard.</font>"
  //: /end
  _GGNAND2 #(4) g1 (.I0(w7), .I1(w4), .Z(c1));   //: @(276,278) /sn:0 /R:3 /w:[ 1 1 0 ]
  _GGNAND2 #(4) g28 (.I0(a0), .I1(b0), .Z(w7));   //: @(248,255) /sn:0 /w:[ 5 5 0 ]
  //: SWITCH g27 (c0) @(157,160) /sn:0 /w:[ 0 ] /st:0
  //: joint g32 (w9) @(275, 199) /w:[ 1 -1 2 4 ]
  _GGNAND2 #(4) g31 (.I0(w9), .I1(c0), .Z(w4));   //: @(278,234) /sn:0 /R:3 /w:[ 5 5 0 ]
  //: SWITCH g25 (a0) @(157,196) /sn:0 /w:[ 0 ] /st:0
  //: joint g29 (a0) @(220, 196) /w:[ 2 -1 1 4 ]
  //: comment g5 @(10,10) /anc:1
  //: /line:"<h3>Group Editing Features</h3> <b>(clipboard operations)</b>"
  //: /line:""
  //: /line:"You can use Cut and Paste operations to cut or copy a group of gates to the clipboard"
  //: /line:"and paste it somewhere else.  Select a group of gates then press the <img src=edit_cut.gif bgcolor=gray> or <img src=edit_copy.gif bgcolor=gray> buttons to"
  //: /line:"cut or copy, respectively, the selected gates to the clipboard.  To paste the contents of the"
  //: /line:"clipboard, first set a <img src=mark.gif> with the left mouse button, then press the <img src=edit_paste.gif bgcolor=gray> button to paste the"
  //: /line:"clipboard contents to that location."
  //: /line:""
  //: /end
  _GGXOR2 #(8) g24 (.I0(w9), .I1(c0), .Z(s0));   //: @(316,202) /sn:0 /w:[ 0 3 0 ]
  _GGXOR2 #(8) g23 (.I0(a0), .I1(b0), .Z(w9));   //: @(241,199) /sn:0 /w:[ 3 3 3 ]
  //: comment g0 @(10,310) /sn:0 /R:14 /anc:1
  //: /line:"<tutorial-navigation>"
  //: /end
  //: SWITCH g26 (b0) @(157,231) /sn:0 /w:[ 0 ] /st:0
  //: joint g30 (b0) @(214, 231) /w:[ -1 2 1 4 ]
  //: joint g33 (c0) @(280, 204) /w:[ 2 1 -1 4 ]

endmodule
//: /netlistEnd

//: /netlistBegin PAGE2
module PAGE2();
//: interface  /sz:(40, 40) /bd:[ ]
reg c0;    //: {0}(18:182,149)(288,149)(288,191){1}
//: {2}(290,193)(313,193){3}
//: {4}(288,195)(288,212){5}
reg a0;    //: {0}(40:182,185)(226,185){1}
//: {2}(230,185)(238,185){3}
//: {4}(228,187)(228,241)(245,241){5}
reg b0;    //: {0}(50:182,220)(220,220){1}
//: {2}(222,218)(222,190)(238,190){3}
//: {4}(222,222)(222,246)(245,246){5}
wire w7;    //: /sn:0 {0}(266,244)(281,244)(281,256){1}
wire s0;    //: {0}(36:334,191)(364,191){1}
wire w4;    //: /sn:0 {0}(286,233)(286,256){1}
wire c1;    //: {0}(99:284,277)(284,296){1}
wire w9;    //: /sn:0 {0}(313,188)(285,188){1}
//: {2}(281,188)(259,188){3}
//: {4}(283,190)(283,212){5}
//: enddecls

  //: LED g37 (s0) @(371,191) /sn:0 /R:3 /w:[ 1 ] /type:0
  //: comment g2 @(609,113) /sn:0 /anc:1
  //: /line:"<font color=green4>Hint: Click in a blank area to"
  //: /line:"unselect a group."
  //: /line:""
  //: /end
  _GGNAND2 #(4) g1 (.I0(w7), .I1(w4), .Z(c1));   //: @(284,267) /sn:0 /R:3 /w:[ 1 1 0 ]
  _GGNAND2 #(4) g28 (.I0(a0), .I1(b0), .Z(w7));   //: @(256,244) /sn:0 /w:[ 5 5 0 ]
  //: SWITCH g27 (c0) @(165,149) /sn:0 /w:[ 0 ] /st:0
  //: joint g32 (w9) @(283, 188) /w:[ 1 -1 2 4 ]
  _GGNAND2 #(4) g31 (.I0(w9), .I1(c0), .Z(w4));   //: @(286,223) /sn:0 /R:3 /w:[ 5 5 0 ]
  //: SWITCH g25 (a0) @(165,185) /sn:0 /w:[ 0 ] /st:0
  //: joint g29 (a0) @(228, 185) /w:[ 2 -1 1 4 ]
  //: comment g5 @(10,10) /anc:1
  //: /line:"<h3>Group Editing Features</h3> <b>(selection and movement)</b>"
  //: /line:""
  //: /line:"To select a group of gates, press and hold the left mouse button above and to the left"
  //: /line:"of the group, then drag the cursor diagonally accross the group to open the selection box."
  //: /line:"Release the mouse when the selection box includes all of the gates you wish to select.  Drag"
  //: /line:"any selected gate to move the entire group.  Try it on the circuit below."
  //: /end
  _GGXOR2 #(8) g24 (.I0(w9), .I1(c0), .Z(s0));   //: @(324,191) /sn:0 /w:[ 0 3 0 ]
  _GGXOR2 #(8) g23 (.I0(a0), .I1(b0), .Z(w9));   //: @(249,188) /sn:0 /w:[ 3 3 3 ]
  //: comment g0 @(10,310) /sn:0 /R:14 /anc:1
  //: /line:"<tutorial-navigation>"
  //: /end
  //: SWITCH g26 (b0) @(165,220) /sn:0 /w:[ 0 ] /st:0
  //: joint g30 (b0) @(222, 220) /w:[ -1 2 1 4 ]
  //: joint g33 (c0) @(288, 193) /w:[ 2 1 -1 4 ]

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

