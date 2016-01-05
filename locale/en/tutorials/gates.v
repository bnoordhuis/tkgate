//: version "2.0-b6"
//: property encoding = "iso8859-1"
//: property locale = "en"
//: property prefix = "_GG"
//: property title = "edit1_tut.v"
//: property useExtBars = 0
//: property discardChanges = 1

`timescale 1ns/1ns

//: /netlistBegin PAGE1
module PAGE1;    //: root_module
//: enddecls

  //: comment g2 @(10,10) /anc:1
  //: /line:"<h3>Editing Gates</h3>"
  //: /line:""
  //: /line:"This chapter will introduce some of the basic circuit editing features of TkGate."
  //: /end
  //: comment g10 @(10,310) /sn:0 /R:14 /anc:1
  //: /line:"<tutorial-navigation>"
  //: /end
  //: comment g0 @(144,69) /sn:0 /anc:1
  //: /line:"<img src=bigdetails.gif>"
  //: /end

endmodule
//: /netlistEnd

//: /netlistBegin PAGE6
module PAGE6();
//: interface  /sz:(40, 40) /bd:[ ]
wire w4;    //: /sn:0 {0}(256,171)(271,171){1}
wire w3;    //: /sn:0 {0}(256,166)(271,166){1}
wire w0;    //: /sn:0 {0}(102,172)(117,172){1}
wire w1;    //: /sn:0 {0}(102,177)(117,177){1}
wire w2;    //: /sn:0 {0}(138,175)(153,175){1}
wire w5;    //: /sn:0 {0}(292,169)(307,169){1}
//: enddecls

  //: comment g4 @(247,195) /sn:0 /anc:1
  //: /line:"Anchored Gate"
  //: /end
  _GGOR2 #(6) g3 (.I0(w3), .I1(w4), .Z(w5));   //: @(282,169) /sn:0 /anc:1 /w:[ 1 1 0 ]
  //: comment g2 @(10,10) /anc:1
  //: /line:"<h3>Editing Gates</h3> <b>(Anchoring)</b>"
  //: /line:""
  //: /line:"You can prevent a gate from being accidentally moved by selecting it and pressing"
  //: /line:"the <img src=anchor.gif bgcolor=gray> button to set an anchor.  You can remove an anchor with the <img src=unanchor.gif bgcolor=gray> button."
  //: /line:"Gates which have been anchored (e.g., the OR gate below) can not be moved until"
  //: /line:"you remove the anchor.  Try moving the gates below."
  //: /line:""
  //: /end
  _GGAND2 #(6) g1 (.I0(w0), .I1(w1), .Z(w2));   //: @(128,175) /sn:0 /w:[ 1 1 0 ]
  //: comment g6 @(587,112) /sn:0 /anc:1
  //: /line:"<font color=green4>Hint: An alternate way to add or"
  //: /line:"remove an anchor on a gate is to"
  //: /line:"right click and change the anchor"
  //: /line:"flag in the popup menu.</font>"
  //: /end
  //: comment g5 @(81,195) /sn:0 /anc:1
  //: /line:"Unanchored Gate"
  //: /end
  //: comment g0 @(10,310) /sn:0 /anc:1
  //: /line:"<tutorial-navigation>"
  //: /end

endmodule
//: /netlistEnd

//: /netlistBegin PAGE3
module PAGE3();
//: interface  /sz:(40, 40) /bd:[ ]
reg c0;    //: {0}(18:118,113)(224,113)(224,155){1}
//: {2}(226,157)(249,157){3}
//: {4}(224,159)(224,169){5}
reg a0;    //: {0}(40:118,149)(162,149){1}
//: {2}(166,149)(174,149){3}
//: {4}(164,151)(164,205)(181,205){5}
reg b0;    //: {0}(50:118,184)(156,184){1}
//: {2}(158,182)(158,154)(174,154){3}
//: {4}(158,186)(158,210)(181,210){5}
wire w7;    //: /sn:0 {0}(202,208)(217,208)(217,220){1}
wire s0;    //: {0}(36:270,155)(300,155){1}
wire w4;    //: /sn:0 {0}(222,190)(222,220){1}
wire c1;    //: {0}(99:220,241)(220,260){1}
wire w9;    //: /sn:0 {0}(249,152)(221,152){1}
//: {2}(217,152)(195,152){3}
//: {4}(219,154)(219,169){5}
//: enddecls

  //: LED g37 (s0) @(307,155) /sn:0 /R:3 /w:[ 1 ] /type:0
  //: comment g2 @(11,10) /anc:1
  //: /line:"<h3>Editing Gates</h3> <b>(Repositioning)</b>"
  //: /line:""
  //: /line:"To move a gate, select it with the <img src=\"mov_curs.gif\" bgcolor=gray> tool and drag it to a new location.  Try"
  //: /line:"it on the sample circuit below."
  //: /line:""
  //: /end
  _GGNAND2 #(4) g1 (.I0(w7), .I1(w4), .Z(c1));   //: @(220,231) /sn:0 /R:3 /w:[ 1 1 0 ]
  _GGNAND2 #(4) g28 (.I0(a0), .I1(b0), .Z(w7));   //: @(192,208) /sn:0 /w:[ 5 5 0 ]
  //: SWITCH g27 (c0) @(101,113) /sn:0 /w:[ 0 ] /st:0
  //: joint g32 (w9) @(219, 152) /w:[ 1 -1 2 4 ]
  _GGNAND2 #(4) g31 (.I0(w9), .I1(c0), .Z(w4));   //: @(222,180) /sn:0 /R:3 /w:[ 5 5 0 ]
  //: SWITCH g25 (a0) @(101,149) /sn:0 /w:[ 0 ] /st:0
  //: joint g29 (a0) @(164, 149) /w:[ 2 -1 1 4 ]
  _GGXOR2 #(8) g24 (.I0(w9), .I1(c0), .Z(s0));   //: @(260,155) /sn:0 /w:[ 0 3 0 ]
  _GGXOR2 #(8) g23 (.I0(a0), .I1(b0), .Z(w9));   //: @(185,152) /sn:0 /w:[ 3 3 3 ]
  //: comment g0 @(10,310) /sn:0 /anc:1
  //: /line:"<tutorial-navigation>"
  //: /end
  //: SWITCH g26 (b0) @(101,184) /sn:0 /w:[ 0 ] /st:0
  //: joint g30 (b0) @(158, 184) /w:[ -1 2 1 4 ]
  //: joint g33 (c0) @(224, 157) /w:[ 2 1 -1 4 ]

endmodule
//: /netlistEnd

//: /netlistBegin PAGE8
module PAGE8();
//: interface  /sz:(40, 40) /bd:[ ]
wire w6;    //: /sn:0 {0}(39,221)(54,221){1}
wire w7;    //: /sn:0 {0}(41,265)(56,265){1}
wire w14;    //: /sn:0 {0}(169,222)(154,222){1}
wire w16;    //: /sn:0 {0}(154,267)(169,267){1}
wire w4;    //: /sn:0 {0}(39,218)(54,218){1}
wire w15;    //: /sn:0 {0}(154,262)(169,262){1}
wire w19;    //: /sn:0 {0}(169,277)(154,277){1}
wire w3;    //: /sn:0 {0}(39,214)(54,214){1}
wire w0;    //: /sn:0 {0}(37,166)(52,166){1}
wire w21;    //: /sn:0 {0}(152,167)(167,167){1}
wire w23;    //: /sn:0 {0}(188,169)(203,169){1}
wire w20;    //: /sn:0 {0}(41,274)(56,274){1}
wire w1;    //: /sn:0 {0}(37,171)(52,171){1}
wire w8;    //: /sn:0 {0}(41,268)(56,268){1}
wire w18;    //: /sn:0 {0}(190,269)(205,269){1}
wire w22;    //: /sn:0 {0}(152,172)(167,172){1}
wire w17;    //: /sn:0 {0}(169,272)(154,272){1}
wire w2;    //: /sn:0 {0}(73,169)(88,169){1}
wire w11;    //: /sn:0 {0}(154,212)(169,212){1}
wire w12;    //: /sn:0 {0}(154,217)(169,217){1}
wire w10;    //: /sn:0 {0}(41,271)(56,271){1}
wire w13;    //: /sn:0 {0}(190,217)(205,217){1}
wire w5;    //: /sn:0 {0}(75,218)(90,218){1}
wire w9;    //: /sn:0 {0}(77,270)(92,270){1}
//: enddecls

  //: comment g8 @(149,126) /sn:0 /anc:1
  //: /line:"gates with"
  //: /line:"extender bars"
  //: /end
  _GGAND4 #(10) g4 (.I0(w7), .I1(w8), .I2(w10), .I3(w20), .Z(w9));   //: @(67,270) /sn:0 /w:[ 1 1 1 1 0 ]
  _GGAND3 #(8) g3 (.I0(w3), .I1(w4), .I2(w6), .Z(w5));   //: @(65,218) /sn:0 /w:[ 1 1 1 0 ]
  //: comment g2 @(11,13) /anc:1
  //: /line:"<h3>Editing Gates</h3> <b>(Adding Inputs)</b>"
  //: /line:""
  //: /line:"Some gates (e.g., AND gates) can have a variable number of inputs or outputs. To add inputs (or"
  //: /line:"outputs) to these gates, select the gate and press the <img src=addport.gif bgcolor=gray> button. Also, some gates may have multiple styles"
  //: /line:"for adding ports that can be selected through the gate properties box.  Try adding inputs to the gates below,"
  //: /line:"or create a new gate and add inputs to it."
  //: /end
  _GGAND2 #(6) g1 (.I0(w0), .I1(w1), .Z(w2));   //: @(63,169) /sn:0 /w:[ 1 1 0 ]
  _GGAND4 #(10) g6 (.I0(w15), .I1(w16), .I2(w17), .I3(w19), .Z(w18));   //: @(180,269) /sn:0 /w:[ 1 1 0 0 0 ] /eb:1
  _GGAND2 #(6) g9 (.I0(w21), .I1(w22), .Z(w23));   //: @(178,169) /sn:0 /w:[ 1 1 0 ] /eb:1
  //: comment g7 @(26,127) /sn:0 /anc:1
  //: /line:"gates without"
  //: /line:"extender bars"
  //: /end
  _GGAND3 #(8) g5 (.I0(w11), .I1(w12), .I2(w14), .Z(w13));   //: @(180,217) /sn:0 /w:[ 1 1 0 0 ] /eb:1
  //: comment g0 @(10,310) /sn:0 /anc:1
  //: /line:"<tutorial-navigation>"
  //: /end

endmodule
//: /netlistEnd

//: /netlistBegin PAGE7
module PAGE7();
//: interface  /sz:(40, 40) /bd:[ ]
//: enddecls

  //: frame g3 @(298,213) /sn:0 /wi:167 /ht:69 /tx:"This is a frame"
  //: comment g2 @(10,10) /anc:1
  //: /line:"<h3>Editing Gates</h3> <b>(Comments and Frames)</b>"
  //: /line:""
  //: /line:"There are two special \"gates\" that you can use for annotating your circuit specification: comments and frames."
  //: /line:"Neither of these \"gates\" has any effect on the simulation results."
  //: /line:""
  //: /line:"Comments are blocks of text such as those used to construct this tutorial.  Comments are created by right clicking"
  //: /line:"and selecting <font color=red2>Make &rarr;   Comment</font> from the popup menu.  When you create a comment, the properties box will"
  //: /line:"appear immediately allowing you to enter the text of the comment.  Comments can contain limited html tags including links"
  //: /line:"to other circuit files, or short cuts to specific gates in your circuit."
  //: /line:""
  //: /line:"Frames are annotated boxes for organzing groups of related circuit elements that don't warrant creating a new module. Open"
  //: /line:"the properties box by double clicking on an edge to set the annotation string.  Click and drag on a corner to resize."
  //: /line:""
  //: /end
  //: comment g1 @(72,233) /sn:0
  //: /line:"This is a comment with"
  //: /line:"some <b>bold</b> text in it."
  //: /end
  //: comment g0 @(10,310) /sn:0 /anc:1
  //: /line:"<tutorial-navigation>"
  //: /end

endmodule
//: /netlistEnd

//: /netlistBegin PAGE5
module PAGE5();
//: interface  /sz:(40, 40) /bd:[ ]
wire w0;    //: /sn:0 {0}(118,149)(133,149){1}
wire w1;    //: /sn:0 {0}(118,154)(133,154){1}
wire w2;    //: /sn:0 {0}(154,152)(169,152){1}
//: enddecls

  //: comment g2 @(10,10) /anc:1
  //: /line:"<h3>Editing Gates</h3> <b>(Rotation)</b>"
  //: /line:""
  //: /line:"The orientation of a gate can be changed by selecting it and pressing the <img src=\"edit_rotate.gif\" bgcolor=gray> button"
  //: /line:"(or <font color=red2>Tab</font> key) to rotate it counter-clockwise and the <img src=\"edit_brotate.gif\" bgcolor=gray> button (or <font color=red2>Shift-Tab</font> key) to"
  //: /line:"rotate it clockwise.  Try it on the AND gate below."
  //: /end
  _GGAND2 #(6) g1 (.I0(w0), .I1(w1), .Z(w2));   //: @(144,152) /sn:0 /w:[ 1 1 0 ]
  //: comment g6 @(567,92) /sn:0 /anc:1
  //: /line:"<font color=green4>Hint: You can also use the <img src=\"rotation0.gif\" bgcolor=gray>"
  //: /line:"button to set the default orientation"
  //: /line:"for newly created gates.</font>"
  //: /end
  //: comment g0 @(10,310) /sn:0 /anc:1
  //: /line:"<tutorial-navigation>"
  //: /end

endmodule
//: /netlistEnd

//: /netlistBegin PAGE4
module PAGE4();
//: interface  /sz:(40, 40) /bd:[ ]
reg c0;    //: {0}(18:125,126)(231,126)(231,168){1}
//: {2}(233,170)(256,170){3}
//: {4}(231,172)(231,183){5}
reg a0;    //: {0}(40:125,162)(169,162){1}
//: {2}(173,162)(181,162){3}
//: {4}(171,164)(171,218)(188,218){5}
reg b0;    //: {0}(50:125,197)(163,197){1}
//: {2}(165,195)(165,167)(181,167){3}
//: {4}(165,199)(165,223)(188,223){5}
wire w7;    //: /sn:0 {0}(209,221)(224,221)(224,233){1}
wire s0;    //: {0}(36:277,168)(307,168){1}
wire w4;    //: /sn:0 {0}(229,204)(229,233){1}
wire c1;    //: {0}(99:227,254)(227,273){1}
wire w9;    //: /sn:0 {0}(256,165)(228,165){1}
//: {2}(224,165)(202,165){3}
//: {4}(226,167)(226,183){5}
//: enddecls

  //: LED g37 (s0) @(314,168) /sn:0 /R:3 /w:[ 1 ] /type:0
  //: comment g2 @(10,10) /anc:1
  //: /line:"<h3>Editing Gates</h3> <b>(Deletion)</b>"
  //: /line:""
  //: /line:"To delete a gate, select it with the <img src=\"mov_curs.gif\" bgcolor=gray> tool and press the <font color=red2>Delete</font> key.  Try it on the sample circuit below."
  //: /end
  _GGNAND2 #(4) g1 (.I0(w7), .I1(w4), .Z(c1));   //: @(227,244) /sn:0 /R:3 /w:[ 1 1 0 ]
  //: comment g10 @(10,310) /sn:0 /R:14 /anc:1
  //: /line:"<tutorial-navigation>"
  //: /end
  _GGNAND2 #(4) g28 (.I0(a0), .I1(b0), .Z(w7));   //: @(199,221) /sn:0 /w:[ 5 5 0 ]
  //: SWITCH g27 (c0) @(108,126) /sn:0 /w:[ 0 ] /st:0
  //: joint g32 (w9) @(226, 165) /w:[ 1 -1 2 4 ]
  _GGNAND2 #(4) g31 (.I0(w9), .I1(c0), .Z(w4));   //: @(229,194) /sn:0 /R:3 /w:[ 5 5 0 ]
  //: SWITCH g25 (a0) @(108,162) /sn:0 /w:[ 0 ] /st:0
  //: joint g29 (a0) @(171, 162) /w:[ 2 -1 1 4 ]
  _GGXOR2 #(8) g24 (.I0(w9), .I1(c0), .Z(s0));   //: @(267,168) /sn:0 /w:[ 0 3 0 ]
  _GGXOR2 #(8) g23 (.I0(a0), .I1(b0), .Z(w9));   //: @(192,165) /sn:0 /w:[ 3 3 3 ]
  //: SWITCH g26 (b0) @(108,197) /sn:0 /w:[ 0 ] /st:0
  //: joint g30 (b0) @(165, 197) /w:[ -1 2 1 4 ]
  //: joint g33 (c0) @(231, 170) /w:[ 2 1 -1 4 ]

endmodule
//: /netlistEnd

//: /netlistBegin PAGE2
module PAGE2();
//: interface  /sz:(40, 40) /bd:[ ]
reg c0;    //: {0}(18:125,126)(231,126)(231,168){1}
//: {2}(233,170)(256,170){3}
//: {4}(231,172)(231,183){5}
reg a0;    //: {0}(40:125,162)(169,162){1}
//: {2}(173,162)(181,162){3}
//: {4}(171,164)(171,218)(188,218){5}
reg b0;    //: {0}(50:125,197)(163,197){1}
//: {2}(165,195)(165,167)(181,167){3}
//: {4}(165,199)(165,223)(188,223){5}
wire w7;    //: /sn:0 {0}(209,221)(224,221)(224,233){1}
wire s0;    //: {0}(36:277,168)(307,168){1}
wire w4;    //: /sn:0 {0}(229,204)(229,233){1}
wire c1;    //: {0}(99:227,254)(227,273){1}
wire w9;    //: /sn:0 {0}(256,165)(228,165){1}
//: {2}(224,165)(202,165){3}
//: {4}(226,167)(226,183){5}
//: enddecls

  //: LED g37 (s0) @(314,168) /sn:0 /R:3 /w:[ 1 ] /type:0
  //: comment g2 @(10,10) /anc:1
  //: /line:"<h3>Editing Gates</h3> <b>(Properties)</b>"
  //: /line:""
  //: /line:"You can also open the properties box of a gate by double clicking on it.  Try"
  //: /line:"double clicking on some of the gates in the circuit below.  Through the properties"
  //: /line:"box you can change the label of a gate and make the label visible or invisible"
  //: /line:"though the \"Hide Name\" property."
  //: /end
  _GGNAND2 #(4) g1 (.I0(w7), .I1(w4), .Z(c1));   //: @(227,244) /sn:0 /R:3 /w:[ 1 1 0 ]
  _GGNAND2 #(4) g28 (.I0(a0), .I1(b0), .Z(w7));   //: @(199,221) /sn:0 /w:[ 5 5 0 ]
  //: SWITCH g27 (c0) @(108,126) /sn:0 /w:[ 0 ] /st:0
  //: joint g32 (w9) @(226, 165) /w:[ 1 -1 2 4 ]
  //: comment g6 @(548,104) /sn:0 /anc:1
  //: /line:"<font color=green4>Hint: Many properties are gate-"
  //: /line:"specific.  Try opening the property"
  //: /line:"boxes of different gate types to see"
  //: /line:"what proprties can be modified.</font>"
  //: /end
  _GGNAND2 #(4) g31 (.I0(w9), .I1(c0), .Z(w4));   //: @(229,194) /sn:0 /R:3 /w:[ 5 5 0 ]
  //: SWITCH g25 (a0) @(108,162) /sn:0 /w:[ 0 ] /st:0
  //: joint g29 (a0) @(171, 162) /w:[ 2 -1 1 4 ]
  _GGXOR2 #(8) g24 (.I0(w9), .I1(c0), .Z(s0));   //: @(267,168) /sn:0 /w:[ 0 3 0 ]
  _GGXOR2 #(8) g23 (.I0(a0), .I1(b0), .Z(w9));   //: @(192,165) /sn:0 /w:[ 3 3 3 ]
  //: comment g0 @(10,310) /sn:0 /anc:1
  //: /line:"<tutorial-navigation>"
  //: /end
  //: SWITCH g26 (b0) @(108,197) /sn:0 /w:[ 0 ] /st:0
  //: joint g30 (b0) @(165, 197) /w:[ -1 2 1 4 ]
  //: joint g33 (c0) @(231, 170) /w:[ 2 1 -1 4 ]

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


//: /builtinBegin
module _GGAND4 #(.Diz(1)) (I0, I1, I2, I3,  Z);
   output  Z;
   input   I0, I1, I2, I3;
   reg  Z;

   assign #Diz Z = ( I0 & I1 & I2 & I3 );
   
endmodule // and
//: /builtinEnd


//: /builtinBegin
module _GGAND2 #(.Diz(1)) (I0, I1,  Z);
   output  Z;
   input   I0, I1;
   reg  Z;

   assign #Diz Z = ( I0 & I1 );
   
endmodule // and
//: /builtinEnd


//: /builtinBegin
module _GGAND3 #(.Diz(1)) (I0, I1, I2,  Z);
   output  Z;
   input   I0, I1, I2;
   reg  Z;

   assign #Diz Z = ( I0 & I1 & I2 );
   
endmodule // and
//: /builtinEnd


//: /builtinBegin
module _GGOR2 #(.Diz(1)) (I0, I1,  Z);
   output  Z;
   input   I0, I1;

     assign #Diz Z = ( I0 | I1 );
   
endmodule // or
//: /builtinEnd

