//: version "2.0-b6"
//: property encoding = "euc-jp"
//: property locale = "ja"
//: property prefix = "_GG"
//: property title = "Welcome to TkGate Page"
//: property discardChanges = 1

`timescale 1ns/1ns

//: /netlistBegin PAGE1
module PAGE1;    //: root_module
//: enddecls

  //: comment g4 @(10,310) /sn:0 /anc:1
  //: /line:"<tutorial-navigation>"
  //: /end
  //: comment g9 @(10,10) /anc:1
  //: /line:"<h3>Advanced Editing Techniques</h3>"
  //: /line:""
  //: /line:"In this chapter you will learn some additional techniques to be more efficient at editing circuits"
  //: /line:"in TkGate."
  //: /end
  //: comment g0 @(150,93) /sn:0 /anc:1
  //: /line:"<img src=bigiface.gif>"
  //: /end

endmodule
//: /netlistEnd

//: /netlistBegin PAGE6
module PAGE6();
//: interface  /sz:(40, 40) /bd:[ ]
wire w24;    //: /sn:0 {0}(319,194)(254,194){1}
//: {2}(252,192)(252,134)(318,134){3}
//: {4}(250,194)(187,194){5}
wire w23;    //: /sn:0 {0}(309,247)(319,247){1}
wire w25;    //: /sn:0 {0}(334,134)(344,134){1}
wire w22;    //: /sn:0 {0}(156,196)(166,196){1}
wire w10;    //: /sn:0 {0}(156,191)(166,191){1}
wire w27;    //: /sn:0 {0}(335,247)(345,247){1}
wire w26;    //: /sn:0 {0}(335,194)(345,194){1}
//: enddecls

  //: comment g4 @(10,310) /sn:0
  //: /line:"<tutorial-navigation>"
  //: /end
  _GGBUF #(4) g28 (.I(w24), .Z(w26));   //: @(325,194) /sn:0 /w:[ 0 0 ]
  //: joint g27 (w24) @(252, 194) /w:[ 1 2 4 -1 ]
  //: comment g9 @(10,9)
  //: /line:"<h3>Wire Editing</h3> <b>(Joint Creation)</b>"
  //: /line:""
  //: /line:"Now try the same thing again, but this time drop the inverter input closer to the AND gate output"
  //: /line:"away from the joint.  This time a new joint is automatically created."
  //: /line:""
  //: /end
  _GGAND2 #(6) g25 (.I0(w10), .I1(w22), .Z(w24));   //: @(177,194) /sn:0 /w:[ 1 1 5 ] /eb:0
  _GGNBUF #(2) g29 (.I(w23), .Z(w27));   //: @(325,247) /sn:0 /w:[ 1 0 ]
  _GGBUF #(4) g26 (.I(w24), .Z(w25));   //: @(324,134) /sn:0 /w:[ 3 0 ]

endmodule
//: /netlistEnd

//: /netlistBegin PAGE3
module PAGE3();
//: interface  /sz:(40, 40) /bd:[ ]
wire w6;    //: /sn:0 {0}(162,164)(202,164)(202,147)(227,147)(227,159)(252,159)(252,209)(178,209)(178,179){1}
wire w4;    //: /sn:0 {0}(131,166)(141,166){1}
wire w0;    //: /sn:0 {0}(131,161)(141,161){1}
//: enddecls

  _GGXOR2 #(8) g8 (.I0(w0), .I1(w4), .Z(w6));   //: @(152,164) /sn:0 /w:[ 1 1 0 ] /eb:0
  //: comment g4 @(10,310) /sn:0 /anc:1
  //: /line:"<tutorial-navigation>"
  //: /end
  //: comment g9 @(10,10) /anc:1
  //: /line:"<h3>Wire Editing</h3> <b>(Automatic Loop Deletion)</b>"
  //: /line:""
  //: /line:"Grab the endpoint of the XOR output wire in the circuit below.  Drag it up until it crosses the"
  //: /line:"horizontal segment next to the gate and release it.  TkGate will automatically remove the loop."
  //: /line:""
  //: /end

endmodule
//: /netlistEnd

//: /netlistBegin PAGE8
module PAGE8();
//: interface  /sz:(40, 40) /bd:[ ]
wire w46;    //: /sn:0 {0}(421,254)(374,254)(374,196){1}
//: {2}(376,194)(397,194)(397,127)(422,127){3}
//: {4}(372,194)(339,194){5}
wire w0;    //: /sn:0 {0}(67,208)(82,208){1}
wire w43;    //: /sn:0 {0}(308,191)(318,191){1}
wire w1;    //: /sn:0 {0}(67,213)(82,213){1}
wire w2;    //: /sn:0 {0}(103,211)(130,211)(130,135){1}
wire w44;    //: /sn:0 {0}(308,196)(318,196){1}
wire w48;    //: /sn:0 {0}(438,127)(448,127){1}
wire w47;    //: /sn:0 {0}(437,254)(447,254){1}
//: enddecls

  //: comment g4 @(10,310) /sn:0 /anc:1
  //: /line:"<tutorial-navigation>"
  //: /end
  _GGXOR2 #(8) g2 (.I0(w0), .I1(w1), .Z(w2));   //: @(93,211) /sn:0 /w:[ 1 1 0 ]
  _GGAND2 #(6) hari (.I0(w43), .I1(w44), .Z(w46));   //: @(329,194) /sn:0 /anc:1 /w:[ 1 1 5 ] /eb:0
  //: joint g50 (w46) @(374, 194) /anc:1 /w:[ 2 -1 4 1 ]
  //: comment g9 @(10,10) /anc:1
  //: /line:"<h3>Wire Editing</h3> <b>(Rerouting)</b>"
  //: /line:""
  //: /line:"TkGate will automatically reroute wires that do not leave a gate or joint on the correct"
  //: /line:"direction. Try grabbing the wire indicated by the arrow in the two examples below and drag"
  //: /line:"them to the left.  In the XOR gate example, two new wire segments are added.  In the joint"
  //: /line:"example, one new segment is added, and the position on the joint is adjusted."
  //: /line:""
  //: /end
  _GGBUF #(4) g48 (.I(w46), .Z(w47));   //: @(427,254) /sn:0 /anc:1 /w:[ 0 0 ]
  //: comment g0 @(405,147) /sn:0
  //: /line:"<font color=blue4 size=7>&lArr;</font>"
  //: /end
  //: comment g12 @(141,164) /sn:0
  //: /line:"<font color=blue4 size=7>&lArr;</font>"
  //: /end
  _GGBUF #(4) g49 (.I(w46), .Z(w48));   //: @(428,127) /sn:0 /anc:1 /w:[ 3 0 ]

endmodule
//: /netlistEnd

//: /netlistBegin PAGE9
module PAGE9();
//: interface  /sz:(40, 40) /bd:[ ]
wire w32;    //: /sn:0 {0}(316,199)(316,214){1}
wire w6;    //: /sn:0 {0}(85,173)(100,173){1}
wire w7;    //: /sn:0 {0}(85,178)(100,178){1}
wire w16;    //: /sn:0 {0}(80,237)(95,237){1}
wire w14;    //: /sn:0 {0}(107,216)(122,216){1}
wire w19;    //: /sn:0 {0}(85,256)(100,256){1}
wire w15;    //: /sn:0 {0}(80,232)(95,232){1}
wire w4;    //: /sn:0 {0}(99,157)(114,157){1}
wire w38;    //: /sn:0 {0}(335,190)(335,205){1}
wire w3;    //: /sn:0 {0}(99,152)(114,152){1}
wire w0;    //: /sn:0 {0}(91,132)(106,132){1}
wire w37;    //: /sn:0 {0}(333,154)(333,169){1}
wire w34;    //: /sn:0 {0}(295,168)(295,183){1}
wire w21;    //: /sn:0 {0}(359,164)(359,179){1}
wire w31;    //: /sn:0 {0}(314,163)(314,178){1}
wire w28;    //: /sn:0 {0}(394,182)(394,197){1}
wire w41;    //: /sn:0 {0}(416,210)(416,225){1}
wire w36;    //: /sn:0 {0}(338,154)(338,169){1}
wire w24;    //: /sn:0 {0}(378,168)(378,183){1}
wire w23;    //: /sn:0 {0}(356,200)(356,215){1}
wire w20;    //: /sn:0 {0}(121,254)(136,254){1}
wire w1;    //: /sn:0 {0}(91,137)(106,137){1}
wire w25;    //: /sn:0 {0}(373,168)(373,183){1}
wire w40;    //: /sn:0 {0}(414,174)(414,189){1}
wire w35;    //: /sn:0 {0}(297,204)(297,219){1}
wire w18;    //: /sn:0 {0}(85,251)(100,251){1}
wire w8;    //: /sn:0 {0}(121,176)(136,176){1}
wire w30;    //: /sn:0 {0}(319,163)(319,178){1}
wire w22;    //: /sn:0 {0}(354,164)(354,179){1}
wire w17;    //: /sn:0 {0}(116,235)(131,235){1}
wire w12;    //: /sn:0 {0}(71,213)(86,213){1}
wire w11;    //: /sn:0 {0}(117,195)(132,195){1}
wire w2;    //: /sn:0 {0}(127,135)(142,135){1}
wire w10;    //: /sn:0 {0}(81,197)(96,197){1}
wire w27;    //: /sn:0 {0}(399,182)(399,197){1}
wire w13;    //: /sn:0 {0}(71,218)(86,218){1}
wire w33;    //: /sn:0 {0}(300,168)(300,183){1}
wire w5;    //: /sn:0 {0}(135,155)(150,155){1}
wire w29;    //: /sn:0 {0}(396,218)(396,233){1}
wire w9;    //: /sn:0 {0}(81,192)(96,192){1}
wire w39;    //: /sn:0 {0}(419,174)(419,189){1}
wire w26;    //: /sn:0 {0}(375,204)(375,219){1}
//: enddecls

  _GGAND2 #(6) g8 (.I0(w21), .I1(w22), .Z(w23));   //: @(356,190) /sn:0 /R:3 /w:[ 1 1 0 ]
  //: comment g4 @(10,310) /sn:0 /anc:1
  //: /line:"<tutorial-navigation>"
  //: /end
  _GGAND2 #(6) g13 (.I0(w33), .I1(w34), .Z(w35));   //: @(297,194) /sn:0 /R:3 /w:[ 1 1 0 ]
  _GGAND2 #(6) g3 (.I0(w9), .I1(w10), .Z(w11));   //: @(107,195) /sn:0 /w:[ 1 1 0 ]
  _GGAND2 #(6) g2 (.I0(w6), .I1(w7), .Z(w8));   //: @(111,176) /sn:0 /w:[ 1 1 0 ]
  _GGAND2 #(6) g1 (.I0(w3), .I1(w4), .Z(w5));   //: @(125,155) /sn:0 /w:[ 1 1 0 ]
  _GGAND2 #(6) g11 (.I0(w27), .I1(w28), .Z(w29));   //: @(396,208) /sn:0 /R:3 /w:[ 1 1 0 ]
  _GGAND2 #(6) g10 (.I0(w24), .I1(w25), .Z(w26));   //: @(375,194) /sn:0 /R:3 /w:[ 1 1 0 ]
  _GGAND2 #(6) g6 (.I0(w15), .I1(w16), .Z(w17));   //: @(106,235) /sn:0 /w:[ 1 1 0 ]
  _GGAND2 #(6) g7 (.I0(w18), .I1(w19), .Z(w20));   //: @(111,254) /sn:0 /w:[ 1 1 0 ]
  //: comment g9 @(10,10) /anc:1
  //: /line:"<h3>Wire Editing</h3> <b>(Gate Alignment)</b>"
  //: /line:""
  //: /line:"You can align a group of gates by selecting them and then pressing the <img src=edit_valgn.gif> to align them vertically,"
  //: /line:"and <img src=edit_halgn.gif> to align them horizontally.  Try it on the groups of gates below."
  //: /end
  _GGAND2 #(6) g15 (.I0(w39), .I1(w40), .Z(w41));   //: @(416,200) /sn:0 /R:3 /w:[ 1 1 0 ]
  _GGAND2 #(6) g14 (.I0(w36), .I1(w37), .Z(w38));   //: @(335,180) /sn:0 /R:3 /w:[ 1 1 0 ]
  _GGAND2 #(6) g5 (.I0(w12), .I1(w13), .Z(w14));   //: @(97,216) /sn:0 /w:[ 1 1 0 ]
  _GGAND2 #(6) g0 (.I0(w0), .I1(w1), .Z(w2));   //: @(117,135) /sn:0 /w:[ 1 1 0 ]
  _GGAND2 #(6) g12 (.I0(w30), .I1(w31), .Z(w32));   //: @(316,189) /sn:0 /R:3 /w:[ 1 1 0 ]

endmodule
//: /netlistEnd

//: /netlistBegin PAGE7
module PAGE7();
//: interface  /sz:(40, 40) /bd:[ ]
wire w7;    //: /sn:0 {0}(85,137)(95,137){1}
wire w19;    //: /sn:0 {0}(322,140)(332,140){1}
wire w21;    //: /sn:0 {0}(321,242)(331,242){1}
wire w20;    //: /sn:0 {0}(305,242)(288,242)(288,140)(306,140){1}
wire w40;    //: /sn:0 {0}(193,243)(203,243){1}
wire w18;    //: /sn:0 {0}(162,245)(172,245){1}
wire w17;    //: /sn:0 {0}(172,240)(153,240)(153,140)(116,140){1}
wire w9;    //: /sn:0 {0}(85,142)(95,142){1}
//: enddecls

  //: comment g4 @(10,310) /sn:0 /anc:1
  //: /line:"<tutorial-navigation>"
  //: /end
  _GGOR2 #(6) g13 (.I0(w17), .I1(w18), .Z(w40));   //: @(183,243) /sn:0 /anc:1 /w:[ 0 1 0 ] /eb:0
  //: comment g1 @(120,178) /sn:0 /anc:1
  //: /line:"<font color=blue4 size=7>&rArr;</font>"
  //: /end
  _GGBUF #(4) g19 (.I(w20), .Z(w19));   //: @(312,140) /sn:0 /anc:1 /w:[ 1 0 ]
  //: comment g9 @(10,10) /anc:1
  //: /line:"<h3>Wire Editing</h3> <b>(Wire Segments)</b>"
  //: /line:""
  //: /line:"Sometime there may not be a free end of two wires you want to connect.  Right click on the"
  //: /line:"wire connected to the AND gate output at the position indicated by the arrow.  Select the"
  //: /line:"<font color=red2>Add Wire Segment</font> option from the popup menu.  You can now connect"
  //: /line:"the newly created wire to the buffer inputs."
  //: /line:""
  //: /end
  _GGAND2 #(6) g5 (.I0(w7), .I1(w9), .Z(w17));   //: @(106,140) /sn:0 /anc:1 /w:[ 1 1 1 ] /eb:0
  _GGBUF #(4) g23 (.I(w20), .Z(w21));   //: @(311,242) /sn:0 /anc:1 /w:[ 0 0 ]

endmodule
//: /netlistEnd

//: /netlistBegin PAGE5
module PAGE5();
//: interface  /sz:(40, 40) /bd:[ ]
wire w23;    //: /sn:0 {0}(309,247)(319,247){1}
wire w24;    //: /sn:0 {0}(319,194)(254,194){1}
//: {2}(252,192)(252,134)(318,134){3}
//: {4}(250,194)(187,194){5}
wire w25;    //: /sn:0 {0}(334,134)(344,134){1}
wire w22;    //: /sn:0 {0}(156,196)(166,196){1}
wire w10;    //: /sn:0 {0}(156,191)(166,191){1}
wire w27;    //: /sn:0 {0}(335,247)(345,247){1}
wire w26;    //: /sn:0 {0}(335,194)(345,194){1}
//: enddecls

  //: comment g4 @(10,310) /sn:0 /anc:1
  //: /line:"<tutorial-navigation>"
  //: /end
  _GGBUF #(4) g28 (.I(w24), .Z(w26));   //: @(325,194) /sn:0 /w:[ 0 0 ]
  //: joint g27 (w24) @(252, 194) /w:[ 1 2 4 -1 ]
  //: comment g9 @(10,9)
  //: /line:"<h3>Wire Editing</h3> <b>(Joint Creation)</b>"
  //: /line:""
  //: /line:"Select the <img src=\"mov_curs.gif\"> tool and drag the input of the inverter in the circuit below.  Drag the input close"
  //: /line:"to the joint in the middle of the AND gate output and release it.  The wire should attach itself to"
  //: /line:"the joint."
  //: /line:""
  //: /end
  _GGAND2 #(6) g25 (.I0(w10), .I1(w22), .Z(w24));   //: @(177,194) /sn:0 /w:[ 1 1 5 ] /eb:0
  _GGNBUF #(2) g29 (.I(w23), .Z(w27));   //: @(325,247) /sn:0 /w:[ 1 0 ]
  _GGBUF #(4) g26 (.I(w24), .Z(w25));   //: @(324,134) /sn:0 /w:[ 3 0 ]

endmodule
//: /netlistEnd

//: /netlistBegin PAGE2
module PAGE2();
//: interface  /sz:(40, 40) /bd:[ ]
wire w16;    //: /sn:0 {0}(313,266)(323,266){1}
wire w14;    //: /sn:0 {0}(292,263)(226,263)(226,200)(245,200)(245,140)(178,140){1}
wire w15;    //: /sn:0 {0}(282,268)(292,268){1}
wire w12;    //: /sn:0 {0}(147,142)(157,142){1}
wire w11;    //: /sn:0 {0}(147,137)(157,137){1}
//: enddecls

  //: comment g4 @(10,310) /sn:0 /anc:1
  //: /line:"<tutorial-navigation>"
  //: /end
  _GGAND2 #(6) g11 (.I0(w14), .I1(w15), .Z(w16));   //: @(303,266) /sn:0 /w:[ 0 1 0 ] /eb:0
  _GGAND2 #(6) g10 (.I0(w11), .I1(w12), .Z(w14));   //: @(168,140) /sn:0 /w:[ 1 1 1 ] /eb:0
  //: comment g9 @(10,10) /anc:1
  //: /line:"<h3>Wire Editing</h3> <b>(Gravity)</b>"
  //: /line:""
  //: /line:"Click on the wire indicated by the arrow in the circuit below and drag it until it is nearly"
  //: /line:"alligned with the other vertical segment.  When you release the wire, \"gravity\" will cause"
  //: /line:"the two vertical segments to snap together into a single segment."
  //: /line:""
  //: /end
  //: comment g0 @(562,122) /sn:0
  //: /line:"<font color=green4>Hint: You can disable the automatic"
  //: /line:"cleanup features in wire editing by"
  //: /line:"holding the Alt key while moving wires.</font>"
  //: /end
  //: comment g12 @(252,158) /sn:0 /anc:1
  //: /line:"<font color=blue4 size=7>&lArr;</font>"
  //: /end

endmodule
//: /netlistEnd

//: /netlistBegin PAGE4
module PAGE4();
//: interface  /sz:(40, 40) /bd:[ ]
wire w6;    //: /sn:0 {0}(325,245)(373,245){1}
//: {2}(377,245)(468,245){3}
//: {4}(375,243)(375,163)(436,163){5}
wire w7;    //: /sn:0 {0}(421,158)(436,158){1}
wire w4;    //: /sn:0 {0}(289,247)(304,247){1}
wire w0;    //: /sn:0 {0}(289,242)(304,242){1}
wire w3;    //: /sn:0 {0}(61,247)(108,247){1}
//: {2}(112,247)(199,247){3}
//: {4}(110,245)(110,161)(169,161){5}
wire w1;    //: /sn:0 {0}(30,244)(40,244){1}
wire w8;    //: /sn:0 {0}(190,159)(200,159){1}
wire w2;    //: /sn:0 {0}(30,249)(40,249){1}
wire w10;    //: /sn:0 {0}(457,161)(472,161){1}
wire w5;    //: /sn:0 {0}(159,156)(169,156){1}
//: enddecls

  //: comment g4 @(10,310) /sn:0 /anc:1
  //: /line:"<tutorial-navigation>"
  //: /end
  //: joint g3 (w3) @(110, 247) /w:[ 2 4 1 -1 ]
  _GGAND2 #(6) g2 (.I0(w5), .I1(w3), .Z(w8));   //: @(180,159) /sn:0 /anc:1 /w:[ 1 5 0 ] /eb:0
  _GGOR2 #(6) g1 (.I0(w0), .I1(w4), .Z(w6));   //: @(315,245) /sn:0 /anc:1 /w:[ 1 1 0 ]
  //: comment g11 @(363,264) /sn:0
  //: /line:"OR Gate Circuit"
  //: /end
  //: comment g10 @(70,271) /sn:0
  //: /line:"AND Gate Circuit"
  //: /end
  //: joint g6 (w6) @(375, 245) /w:[ 2 4 1 -1 ]
  //: comment g9 @(10,10) /anc:1
  //: /line:"<h3>Wire Editing</h3> <b>(Automatic Wire Cleanup)</b>"
  //: /line:""
  //: /line:"Select the <img src=\"cut_curs.gif\"> tool and cut the wire at the position indicated by the arrow in the AND Gate"
  //: /line:"Circuit.  TkGate will automatically remove the wire joint.  Now cut the wire at the position indicated"
  //: /line:"by the arrow in the OR Gate Circuit.  The free wire end is automatically deleted."
  //: /line:""
  //: /end
  //: comment g7 @(413,213) /sn:0 /anc:1
  //: /line:"<font color=blue4 size=7>&dArr;</font>"
  //: /end
  _GGOR2 #(6) g5 (.I0(w7), .I1(w6), .Z(w10));   //: @(447,161) /sn:0 /anc:1 /w:[ 1 5 0 ]
  _GGAND2 #(6) g0 (.I0(w1), .I1(w2), .Z(w3));   //: @(51,247) /sn:0 /anc:1 /w:[ 1 1 0 ] /eb:0
  //: comment g12 @(123,184) /sn:0 /anc:1
  //: /line:"<font color=blue4 size=7>&lArr;</font>"
  //: /end

endmodule
//: /netlistEnd

//: /netlistBegin PAGE10
module PAGE10();
//: interface  /sz:(40, 40) /bd:[ ]
reg ithaca;    //: {0}(18:215,183)(259,183){1}
//: {2}(263,183)(271,183){3}
//: {4}(261,185)(261,238)(278,238){5}
reg crete;    //: {0}(7:215,136)(359,136)(359,189){1}
//: {2}(361,191)(416,191){3}
//: {4}(359,193)(359,203){5}
reg toledo;    //: {0}(-21:213,243)(254,243){1}
//: {2}(258,243)(278,243){3}
//: {4}(256,241)(256,188)(271,188){5}
wire styx;    //: {0}(99:355,275)(355,295){1}
wire athens;    //: {0}(416,186)(356,186){1}
//: {2}(71:352,186)(292,186){3}
//: {4}(354,188)(354,203){5}
wire pheneos;    //: {0}(76:357,224)(357,254){1}
wire olympus;    //: {0}(-15:437,189)(506,189){1}
wire delphi;    //: {0}(-22:299,241)(352,241)(352,254){1}
//: enddecls

  //: comment g4 @(10,310) /sn:0 /anc:1
  //: /line:"<tutorial-navigation>"
  //: /end
  //: LED g37 (olympus) @(513,189) /sn:0 /R:3 /w:[ 1 ] /type:0
  _GGXOR2 #(8) Athena (.I0(ithaca), .I1(toledo), .Z(athens));   //: @(282,186) /w:[ 3 5 3 ] /eb:0
  _GGNAND2 #(4) Hermes (.I0(athens), .I1(crete), .Z(pheneos));   //: @(357,214) /R:3 /w:[ 5 5 0 ] /eb:0
  _GGNAND2 #(4) Hades (.I0(delphi), .I1(pheneos), .Z(styx));   //: @(355,265) /R:3 /w:[ 1 1 0 ] /eb:0
  _GGXOR2 #(8) Zeus (.I0(athens), .I1(crete), .Z(olympus));   //: @(427,189) /w:[ 0 3 0 ] /eb:0
  //: joint g32 (athens) @(354, 186) /w:[ 1 -1 2 4 ]
  //: comment g9 @(10,10) /anc:1
  //: /line:"<h3>Wire Editing</h3> <b>(Search)</b>"
  //: /line:""
  //: /line:"You can search your circuit for a wire or gate with a specific name by using the search tool.  Click"
  //: /line:"on the <img src=edit_find.gif bgcolor=gray> tool to open it.  The list of matches will by dynamically updated as you type the target"
  //: /line:"string.  Double click on an entry in the <font color=red2>Search Results</font> to go to that object.  Try using the search"
  //: /line:"tool to search for some of the wire and gate names in the circuit below."
  //: /line:""
  //: /end
  //: joint g29 (ithaca) @(261, 183) /w:[ 2 -1 1 4 ]
  //: SWITCH Thesus (crete) @(198,136) /w:[ 0 ] /st:0
  //: SWITCH Bob (toledo) @(196,243) /w:[ 0 ] /st:0
  _GGNAND2 #(4) Apollo (.I0(ithaca), .I1(toledo), .Z(delphi));   //: @(289,241) /w:[ 5 3 0 ] /eb:0
  //: SWITCH Odyseus (ithaca) @(198,183) /w:[ 0 ] /st:0
  //: comment g0 @(588,148) /sn:0
  //: /line:"<font color=green4>Hint: The search tool will look"
  //: /line:"in all modules of your circuit for"
  //: /line:"the target string.  Try searching for"
  //: /line:"a gate named \"hari\".</font>"
  //: /end
  //: joint g30 (toledo) @(256, 243) /w:[ 2 4 1 -1 ]
  //: joint g33 (crete) @(359, 191) /w:[ 2 1 -1 4 ]

endmodule
//: /netlistEnd

//: /netlistBegin OLD
module OLD();
//: interface  /sz:(40, 40) /bd:[ ]
wire w6;    //: /sn:0 {0}(476,150)(516,150)(516,133)(541,133)(541,145)(566,145)(566,195)(492,195)(492,165){1}
wire w32;    //: /sn:0 {0}(624,293)(634,293){1}
wire w7;    //: /sn:0 {0}(470,428)(480,428){1}
wire w46;    //: /sn:0 {0}(724,568)(681,568)(681,559){1}
//: {2}(683,557)(697,557)(697,519)(722,519){3}
//: {4}(679,557)(646,557){5}
wire w14;    //: /sn:0 {0}(522,98)(512,98)(512,79)(531,79)(531,46)(478,46){1}
wire w16;    //: /sn:0 {0}(543,101)(553,101){1}
wire w4;    //: /sn:0 {0}(445,152)(455,152){1}
wire w15;    //: /sn:0 {0}(512,103)(522,103){1}
wire w19;    //: /sn:0 {0}(604,430)(614,430){1}
wire w38;    //: /sn:0 {0}(747,392)(757,392){1}
wire w0;    //: /sn:0 {0}(445,147)(455,147){1}
wire w3;    //: /sn:0 {0}(475,285)(516,285){1}
//: {2}(520,285)(573,285){3}
//: {4}(518,283)(518,239)(548,239){5}
wire w37;    //: /sn:0 {0}(721,392)(731,392){1}
wire w34;    //: /sn:0 {0}(744,361)(754,361){1}
wire w43;    //: /sn:0 {0}(615,554)(625,554){1}
wire w21;    //: /sn:0 {0}(604,468)(614,468){1}
wire w31;    //: /sn:0 {0}(624,288)(634,288){1}
wire w28;    //: /sn:0 {0}(655,291)(696,291){1}
//: {2}(700,291)(753,291){3}
//: {4}(698,289)(698,245)(728,245){5}
wire w24;    //: /sn:0 {0}(547,360)(524,360){1}
//: {2}(522,358)(522,329)(548,329){3}
//: {4}(520,360)(482,360){5}
wire w20;    //: /sn:0 {0}(588,468)(576,468)(576,430)(588,430){1}
wire w36;    //: /sn:0 {0}(632,363)(642,363){1}
wire w41;    //: /sn:0 {0}(492,556)(502,556){1}
wire w23;    //: /sn:0 {0}(537,388)(547,388){1}
wire w1;    //: /sn:0 {0}(444,282)(454,282){1}
wire w25;    //: /sn:0 {0}(564,329)(574,329){1}
wire w18;    //: /sn:0 {0}(523,477)(533,477){1}
wire w40;    //: /sn:0 {0}(554,475)(564,475){1}
wire w8;    //: /sn:0 {0}(569,237)(579,237){1}
wire w35;    //: /sn:0 {0}(632,358)(642,358){1}
wire w30;    //: /sn:0 {0}(749,243)(759,243){1}
wire w17;    //: /sn:0 {0}(533,472)(517,472)(517,431)(501,431){1}
wire w22;    //: /sn:0 {0}(451,362)(461,362){1}
wire w11;    //: /sn:0 {0}(447,43)(457,43){1}
wire w12;    //: /sn:0 {0}(447,48)(457,48){1}
wire w44;    //: /sn:0 {0}(615,559)(625,559){1}
wire w2;    //: /sn:0 {0}(444,287)(454,287){1}
wire w10;    //: /sn:0 {0}(451,357)(461,357){1}
wire w13;    //: /sn:0 {0}(492,551)(502,551){1}
wire w27;    //: /sn:0 {0}(563,388)(573,388){1}
wire w33;    //: /sn:0 {0}(728,361)(705,361){1}
//: {2}(703,359)(703,330)(729,330){3}
//: {4}(701,361)(663,361){5}
wire w5;    //: /sn:0 {0}(538,234)(548,234){1}
wire w48;    //: /sn:0 {0}(738,519)(748,519){1}
wire w29;    //: /sn:0 {0}(718,240)(728,240){1}
wire w47;    //: /sn:0 {0}(740,568)(750,568){1}
wire w42;    //: /sn:0 {0}(523,554)(541,554)(541,518){1}
wire w9;    //: /sn:0 {0}(470,433)(480,433){1}
wire w26;    //: /sn:0 {0}(563,360)(573,360){1}
wire w39;    //: /sn:0 {0}(745,330)(755,330){1}
//: enddecls

  _GGXOR2 #(8) g8 (.I0(w0), .I1(w4), .Z(w6));   //: @(466,150) /sn:0 /w:[ 1 1 0 ] /eb:0
  //: comment g4 @(10,552) /sn:0 /R:14
  //: /line:"<font color=gray>&lt;PREVIOUS</font>    <a href=\"index.v\">UP</a>   <a href=\"module:PART1\">NEXT></a>"
  //: /end
  _GGOR2 #(6) g13 (.I0(w17), .I1(w18), .Z(w40));   //: @(544,475) /sn:0 /w:[ 0 1 0 ] /eb:0
  //: joint g3 (w3) @(518, 285) /w:[ 2 4 1 -1 ]
  //: frame g34 @(437,317) /sn:0 /wi:160 /ht:88 /tx:"Fig. 4a"
  //: joint g37 (w33) @(703, 361) /w:[ 1 2 4 -1 ]
  //: frame g51 @(442,505) /sn:0 /wi:145 /ht:74 /tx:"Fig. 6a"
  _GGAND2 #(6) g2 (.I0(w5), .I1(w3), .Z(w8));   //: @(559,237) /sn:0 /w:[ 1 5 0 ] /eb:0
  //: comment g1 @(542,55) /sn:0
  //: /line:"&lt;---"
  //: /end
  _GGAND2 #(6) g11 (.I0(w14), .I1(w15), .Z(w16));   //: @(533,101) /sn:0 /w:[ 0 1 0 ] /eb:0
  _GGAND2 #(6) g10 (.I0(w11), .I1(w12), .Z(w14));   //: @(468,46) /sn:0 /w:[ 1 1 1 ] /eb:0
  _GGBUF #(4) g28 (.I(w24), .Z(w26));   //: @(553,360) /sn:0 /w:[ 0 0 ]
  //: joint g50 (w46) @(681, 557) /w:[ 2 -1 4 1 ]
  _GGBUF #(4) g19 (.I(w20), .Z(w19));   //: @(594,430) /sn:0 /w:[ 1 0 ]
  //: joint g27 (w24) @(522, 360) /w:[ 1 2 4 -1 ]
  _GGAND2 #(6) g32 (.I0(w29), .I1(w28), .Z(w30));   //: @(739,243) /sn:0 /w:[ 1 5 0 ] /eb:0
  _GGAND2 #(6) g38 (.I0(w35), .I1(w36), .Z(w33));   //: @(653,361) /sn:0 /w:[ 1 1 5 ] /eb:0
  //: comment g9 @(10,10) /anc:1
  //: /line:"<h3>Wire Editing</h3>"
  //: /line:""
  //: /line:"1) Click on the wire in Fig. 1, drag it until it is nearly alligned"
  //: /line:"with the other vertical segment and release it.  \"Gravity\" will cause"
  //: /line:"them to snap together into a single segment."
  //: /line:""
  //: /line:"2) Grab the endpoint of the XOR output wire in Fig. 2, drag it up so that"
  //: /line:"it crosses the horizontal segment next to the gate and release it.  TkGate"
  //: /line:"will automatically remove the loop."
  //: /line:""
  //: /line:"3) Press <img src=\"cut_curs.gif\"> and click on the vertical wire segment in Fig. 3a.  TkGate will"
  //: /line:"automatically remove the wire joint.  Now, click at the point indicated by"
  //: /line:"the arrow in Fig. 3b.  The free wire end is automatically deleted."
  //: /line:""
  //: /line:"4) Press <img src=\"mov_curs.gif\"> then select and drag the input of the bottom buffer in Fig. 4a"
  //: /line:"next to the wire joint and release it to connect it to the joint.  Try again in"
  //: /line:"Fig. 4b, this time releasing it between the AND gate and the joint to"
  //: /line:"create a new joint."
  //: /line:""
  //: /line:"5) Right click on the vertical segment indicated by the arrow in Fig. 5"
  //: /line:"and select <font color=blue>Add Wire Segment</font> from the popup menu.  Grab the new wire"
  //: /line:"end and drag it to the other horizontal segment and release it."
  //: /line:""
  //: /line:"6) In Figs. 6a and 6b, select the wire at the point indicated by the"
  //: /line:"arrows and drag them left past the output point of the AND gate and"
  //: /line:"past the position of the joint.  Tkgate will reroute the wires as necessary."
  //: /line:""
  //: /line:""
  //: /line:""
  //: /line:"  <font color=green4>Hint: You can disable the automatic cleanup features in wire"
  //: /line:"  editing by holding the Alt key while moving wires.</font>"
  //: /end
  //: joint g31 (w28) @(698, 291) /w:[ 2 4 1 -1 ]
  _GGAND2 #(6) g39 (.I0(w13), .I1(w41), .Z(w42));   //: @(513,554) /sn:0 /w:[ 1 1 0 ] /eb:0
  //: frame g43 @(617,222) /sn:0 /wi:160 /ht:85 /tx:"Fig. 3b"
  _GGBUF #(4) g48 (.I(w46), .Z(w47));   //: @(730,568) /sn:0 /w:[ 0 0 ]
  _GGAND2 #(6) g25 (.I0(w10), .I1(w22), .Z(w24));   //: @(472,360) /sn:0 /w:[ 1 1 5 ] /eb:0
  _GGBUF #(4) g29 (.I(w23), .Z(w27));   //: @(553,388) /sn:0 /w:[ 1 0 ]
  //: frame g42 @(437,221) /sn:0 /wi:163 /ht:83 /tx:"Fig. 3a"
  //: frame g52 @(597,507) /sn:0 /wi:189 /ht:73 /tx:"Fig. 6b"
  _GGAND2 #(6) g5 (.I0(w7), .I1(w9), .Z(w17));   //: @(491,431) /sn:0 /w:[ 1 1 1 ] /eb:0
  _GGBUF #(4) g44 (.I(w37), .Z(w38));   //: @(737,392) /sn:0 /w:[ 1 0 ]
  _GGAND2 #(6) g47 (.I0(w43), .I1(w44), .Z(w46));   //: @(636,557) /sn:0 /w:[ 1 1 5 ] /eb:0
  //: comment g24 @(485,442) /sn:0
  //: /line:"--->"
  //: /end
  _GGBUF #(4) g36 (.I(w33), .Z(w34));   //: @(734,361) /sn:0 /w:[ 0 0 ]
  _GGBUF #(4) g23 (.I(w20), .Z(w21));   //: @(594,468) /sn:0 /w:[ 0 0 ]
  //: frame g41 @(436,127) /sn:0 /wi:141 /ht:77 /tx:"Fig. 2"
  //: frame g40 @(434,30) /sn:0 /wi:147 /ht:86 /tx:"Fig. 1"
  _GGAND2 #(6) g0 (.I0(w1), .I1(w2), .Z(w3));   //: @(465,285) /sn:0 /w:[ 1 1 0 ] /eb:0
  _GGBUF #(4) g26 (.I(w24), .Z(w25));   //: @(554,329) /sn:0 /w:[ 3 0 ]
  //: frame g35 @(618,318) /sn:0 /wi:159 /ht:88 /tx:"Fig. 4b"
  _GGBUF #(4) g45 (.I(w33), .Z(w39));   //: @(735,330) /sn:0 /w:[ 3 0 ]
  //: frame g46 @(440,416) /sn:0 /wi:190 /ht:75 /tx:"Fig. 5"
  //: comment g30 @(723,257) /sn:0
  //: /line:"|"
  //: /line:"v"
  //: /end
  _GGAND2 #(6) g33 (.I0(w31), .I1(w32), .Z(w28));   //: @(645,291) /sn:0 /w:[ 1 1 0 ] /eb:0
  _GGBUF #(4) g49 (.I(w46), .Z(w48));   //: @(728,519) /sn:0 /w:[ 3 0 ]

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
module _GGBUF #(.Diz(1)) (I, Z);
   input  I;
   output  Z;

   assign #Diz Z = I;
   
endmodule // buf
//: /builtinEnd


//: /builtinBegin
module _GGNBUF #(.Diz(1)) (I, Z);
   input  I;
   output  Z;

   assign #Diz Z = ~I;
   
endmodule // buf
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
module _GGOR2 #(.Diz(1)) (I0, I1,  Z);
   output  Z;
   input   I0, I1;

     assign #Diz Z = ( I0 | I1 );
   
endmodule // or
//: /builtinEnd

