//: version "2.0-b6"
//: property encoding = "euc-jp"
//: property locale = "ja"
//: property prefix = "_GG"
//: property title = "edit1_tut.v"
//: property useExtBars = 0
//: property discardChanges = 1

`timescale 1ns/1ns

//: /netlistBegin PAGE1
module PAGE1;    //: root_module
//: enddecls

  //: comment g2 @(10,10) /anc:1
  //: /line:"<h3>Editing Wires</h3>"
  //: /line:""
  //: /line:"This chapter will introduce the basic wire editing features of TkGate."
  //: /end
  //: comment g10 @(10,310) /sn:0 /R:14 /anc:1
  //: /line:"<tutorial-navigation>"
  //: /end
  //: comment g0 @(185,114) /sn:0 /anc:1
  //: /line:"<img src=netprops.gif>"
  //: /end

endmodule
//: /netlistEnd

//: /netlistBegin PAGE6
module PAGE6();
//: interface  /sz:(40, 40) /bd:[ ]
reg c0;    //: {0}(18:155,153)(261,153)(261,195){1}
//: {2}(263,197)(286,197){3}
//: {4}(261,199)(261,209){5}
reg a0;    //: {0}(40:155,189)(199,189){1}
//: {2}(203,189)(211,189){3}
//: {4}(201,191)(201,245)(218,245){5}
reg b0;    //: {0}(50:155,224)(193,224){1}
//: {2}(195,222)(195,194)(211,194){3}
//: {4}(195,226)(195,250)(218,250){5}
wire w7;    //: /sn:0 {0}(239,248)(254,248)(254,260){1}
wire s0;    //: {0}(36:307,195)(337,195){1}
wire w4;    //: /sn:0 {0}(259,230)(259,260){1}
wire c1;    //: {0}(99:257,281)(257,300){1}
wire w9;    //: /sn:0 {0}(286,192)(258,192){1}
//: {2}(254,192)(232,192){3}
//: {4}(256,194)(256,209){5}
//: enddecls

  //: LED g37 (s0) @(344,195) /sn:0 /R:3 /w:[ 1 ] /type:0
  //: comment g2 @(11,10)
  //: /line:"<h3>Basic Editing Modes</h3> <b>(wire labels)</b>"
  //: /line:""
  //: /line:"Wire label editing is performed with the <img src=mov_curs.gif bgcolor=gray> tool.  Select this tool if it is not already selected.  Existing"
  //: /line:"wire labels can be moved by left clicking and dragging the label to a new position.  Wire labels can only"
  //: /line:"be moved to a different locations on the wire from which it was grabbed.  If you drop the label too far from"
  //: /line:"its wire, the label will be discarded.  To display a new label right click on the wire and choose the <font color=red2>Show"
  //: /line:"Label</font> option.  You can also hide a specific label (<font color=red2>Hide Label</font>) or hide all labels for a wire (<font color=red2>Hide All Labels</font>)."
  //: /end
  _GGNAND2 #(4) g1 (.I0(w7), .I1(w4), .Z(c1));   //: @(257,271) /sn:0 /R:3 /w:[ 1 1 0 ]
  _GGNAND2 #(4) g28 (.I0(a0), .I1(b0), .Z(w7));   //: @(229,248) /sn:0 /w:[ 5 5 0 ]
  //: joint g32 (w9) @(256, 192) /w:[ 1 -1 2 4 ]
  //: SWITCH g27 (c0) @(138,153) /sn:0 /w:[ 0 ] /st:0
  _GGNAND2 #(4) g31 (.I0(w9), .I1(c0), .Z(w4));   //: @(259,220) /sn:0 /R:3 /w:[ 5 5 0 ]
  //: joint g29 (a0) @(201, 189) /w:[ 2 -1 1 4 ]
  //: SWITCH g25 (a0) @(138,189) /sn:0 /w:[ 0 ] /st:0
  _GGXOR2 #(8) g24 (.I0(w9), .I1(c0), .Z(s0));   //: @(297,195) /sn:0 /w:[ 0 3 0 ]
  _GGXOR2 #(8) g23 (.I0(a0), .I1(b0), .Z(w9));   //: @(222,192) /sn:0 /w:[ 3 3 3 ]
  //: comment g0 @(10,310) /sn:0 /anc:1
  //: /line:"<tutorial-navigation>"
  //: /end
  //: SWITCH g26 (b0) @(138,224) /sn:0 /w:[ 0 ] /st:0
  //: joint g33 (c0) @(261, 197) /w:[ 2 1 -1 4 ]
  //: joint g30 (b0) @(195, 224) /w:[ -1 2 1 4 ]

endmodule
//: /netlistEnd

//: /netlistBegin advanced
module advanced();
//: interface  /sz:(40, 40) /bd:[ ]
wire w6;    //: /sn:0 {0}(483,145)(523,145)(523,128)(548,128)(548,140)(573,140)(573,190)(499,190)(499,160){1}
wire w32;    //: /sn:0 {0}(631,288)(641,288){1}
wire w7;    //: /sn:0 {0}(477,423)(487,423){1}
wire w46;    //: /sn:0 {0}(731,563)(688,563)(688,554){1}
//: {2}(690,552)(704,552)(704,514)(729,514){3}
//: {4}(686,552)(653,552){5}
wire w14;    //: /sn:0 {0}(529,93)(519,93)(519,74)(538,74)(538,41)(485,41){1}
wire w16;    //: /sn:0 {0}(550,96)(560,96){1}
wire w4;    //: /sn:0 {0}(452,147)(462,147){1}
wire w15;    //: /sn:0 {0}(519,98)(529,98){1}
wire w19;    //: /sn:0 {0}(611,425)(621,425){1}
wire w38;    //: /sn:0 {0}(754,387)(764,387){1}
wire w3;    //: /sn:0 {0}(482,280)(523,280){1}
//: {2}(527,280)(580,280){3}
//: {4}(525,278)(525,234)(555,234){5}
wire w0;    //: /sn:0 {0}(452,142)(462,142){1}
wire w37;    //: /sn:0 {0}(728,387)(738,387){1}
wire w34;    //: /sn:0 {0}(751,356)(761,356){1}
wire w21;    //: /sn:0 {0}(611,463)(621,463){1}
wire w43;    //: /sn:0 {0}(622,549)(632,549){1}
wire w31;    //: /sn:0 {0}(631,283)(641,283){1}
wire w28;    //: /sn:0 {0}(662,286)(703,286){1}
//: {2}(707,286)(760,286){3}
//: {4}(705,284)(705,240)(735,240){5}
wire w20;    //: /sn:0 {0}(595,463)(583,463)(583,425)(595,425){1}
wire w24;    //: /sn:0 {0}(554,355)(531,355){1}
//: {2}(529,353)(529,324)(555,324){3}
//: {4}(527,355)(489,355){5}
wire w23;    //: /sn:0 {0}(544,383)(554,383){1}
wire w41;    //: /sn:0 {0}(499,551)(509,551){1}
wire w36;    //: /sn:0 {0}(639,358)(649,358){1}
wire w1;    //: /sn:0 {0}(451,277)(461,277){1}
wire w25;    //: /sn:0 {0}(571,324)(581,324){1}
wire w40;    //: /sn:0 {0}(561,470)(571,470){1}
wire w18;    //: /sn:0 {0}(530,472)(540,472){1}
wire w8;    //: /sn:0 {0}(576,232)(586,232){1}
wire w35;    //: /sn:0 {0}(639,353)(649,353){1}
wire w30;    //: /sn:0 {0}(756,238)(766,238){1}
wire w17;    //: /sn:0 {0}(540,467)(524,467)(524,426)(508,426){1}
wire w22;    //: /sn:0 {0}(458,357)(468,357){1}
wire w2;    //: /sn:0 {0}(451,282)(461,282){1}
wire w11;    //: /sn:0 {0}(454,38)(464,38){1}
wire w12;    //: /sn:0 {0}(454,43)(464,43){1}
wire w44;    //: /sn:0 {0}(622,554)(632,554){1}
wire w10;    //: /sn:0 {0}(458,352)(468,352){1}
wire w27;    //: /sn:0 {0}(570,383)(580,383){1}
wire w13;    //: /sn:0 {0}(499,546)(509,546){1}
wire w5;    //: /sn:0 {0}(545,229)(555,229){1}
wire w33;    //: /sn:0 {0}(735,356)(712,356){1}
//: {2}(710,354)(710,325)(736,325){3}
//: {4}(708,356)(670,356){5}
wire w48;    //: /sn:0 {0}(745,514)(755,514){1}
wire w29;    //: /sn:0 {0}(725,235)(735,235){1}
wire w47;    //: /sn:0 {0}(747,563)(757,563){1}
wire w9;    //: /sn:0 {0}(477,428)(487,428){1}
wire w42;    //: /sn:0 {0}(530,549)(548,549)(548,513){1}
wire w26;    //: /sn:0 {0}(570,355)(580,355){1}
wire w39;    //: /sn:0 {0}(752,325)(762,325){1}
//: enddecls

  _GGXOR2 #(8) g8 (.I0(w0), .I1(w4), .Z(w6));   //: @(473,145) /sn:0 /w:[ 1 1 0 ]
  //: comment g4 @(16,548) /sn:0 /R:14
  //: /line:"<font color=gray>&lt;PREVIOUS</font>    <a href=\"index.v\">UP</a>   <a href=\"module:PART1\">NEXT></a>"
  //: /end
  _GGOR2 #(6) g13 (.I0(w17), .I1(w18), .Z(w40));   //: @(551,470) /sn:0 /w:[ 0 1 0 ]
  //: joint g3 (w3) @(525, 280) /w:[ 2 4 1 -1 ]
  //: frame g34 @(444,312) /sn:0 /anc:1 /wi:160 /ht:88 /tx:"Fig. 4a"
  //: joint g37 (w33) @(710, 356) /w:[ 1 2 4 -1 ]
  //: frame g51 @(449,500) /sn:0 /anc:1 /wi:145 /ht:74 /tx:"Fig. 6a"
  _GGAND2 #(6) g2 (.I0(w5), .I1(w3), .Z(w8));   //: @(566,232) /sn:0 /w:[ 1 5 0 ]
  //: comment g1 @(549,50) /sn:0
  //: /line:"&lt;---"
  //: /end
  _GGAND2 #(6) g11 (.I0(w14), .I1(w15), .Z(w16));   //: @(540,96) /sn:0 /w:[ 0 1 0 ]
  _GGAND2 #(6) g10 (.I0(w11), .I1(w12), .Z(w14));   //: @(475,41) /sn:0 /w:[ 1 1 1 ]
  _GGBUF #(4) g28 (.I(w24), .Z(w26));   //: @(560,355) /sn:0 /w:[ 0 0 ]
  //: joint g50 (w46) @(688, 552) /w:[ 2 -1 4 1 ]
  _GGBUF #(4) g19 (.I(w20), .Z(w19));   //: @(601,425) /sn:0 /w:[ 1 0 ]
  //: joint g27 (w24) @(529, 355) /w:[ 1 2 4 -1 ]
  _GGAND2 #(6) g32 (.I0(w29), .I1(w28), .Z(w30));   //: @(746,238) /sn:0 /w:[ 1 5 0 ]
  _GGAND2 #(6) g38 (.I0(w35), .I1(w36), .Z(w33));   //: @(660,356) /sn:0 /w:[ 1 1 5 ]
  //: comment g9 @(23,10) /anc:1
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
  //: joint g31 (w28) @(705, 286) /w:[ 2 4 1 -1 ]
  _GGAND2 #(6) g39 (.I0(w13), .I1(w41), .Z(w42));   //: @(520,549) /sn:0 /w:[ 1 1 0 ]
  //: frame g43 @(624,217) /sn:0 /anc:1 /wi:160 /ht:85 /tx:"Fig. 3b"
  _GGBUF #(4) g48 (.I(w46), .Z(w47));   //: @(737,563) /sn:0 /w:[ 0 0 ]
  _GGAND2 #(6) g25 (.I0(w10), .I1(w22), .Z(w24));   //: @(479,355) /sn:0 /w:[ 1 1 5 ]
  _GGBUF #(4) g29 (.I(w23), .Z(w27));   //: @(560,383) /sn:0 /w:[ 1 0 ]
  //: frame g42 @(444,216) /sn:0 /anc:1 /wi:163 /ht:83 /tx:"Fig. 3a"
  //: frame g52 @(604,502) /sn:0 /anc:1 /wi:189 /ht:73 /tx:"Fig. 6b"
  _GGAND2 #(6) g5 (.I0(w7), .I1(w9), .Z(w17));   //: @(498,426) /sn:0 /w:[ 1 1 1 ]
  _GGBUF #(4) g44 (.I(w37), .Z(w38));   //: @(744,387) /sn:0 /w:[ 1 0 ]
  _GGAND2 #(6) g47 (.I0(w43), .I1(w44), .Z(w46));   //: @(643,552) /sn:0 /w:[ 1 1 5 ]
  //: comment g24 @(492,437) /sn:0 /anc:1
  //: /line:"--->"
  //: /end
  _GGBUF #(4) g36 (.I(w33), .Z(w34));   //: @(741,356) /sn:0 /w:[ 0 0 ]
  _GGBUF #(4) g23 (.I(w20), .Z(w21));   //: @(601,463) /sn:0 /w:[ 0 0 ]
  //: frame g41 @(442,122) /sn:0 /anc:1 /wi:142 /ht:86 /tx:"Fig. 2"
  //: frame g40 @(441,25) /sn:0 /anc:1 /wi:147 /ht:86 /tx:"Fig. 1"
  _GGAND2 #(6) g0 (.I0(w1), .I1(w2), .Z(w3));   //: @(472,280) /sn:0 /w:[ 1 1 0 ]
  _GGBUF #(4) g26 (.I(w24), .Z(w25));   //: @(561,324) /sn:0 /w:[ 3 0 ]
  //: frame g35 @(625,313) /sn:0 /anc:1 /wi:159 /ht:88 /tx:"Fig. 4b"
  _GGBUF #(4) g45 (.I(w33), .Z(w39));   //: @(742,325) /sn:0 /w:[ 3 0 ]
  //: frame g46 @(447,411) /sn:0 /anc:1 /wi:190 /ht:75 /tx:"Fig. 5"
  //: comment g30 @(730,252) /sn:0 /anc:1
  //: /line:"|"
  //: /line:"v"
  //: /end
  _GGAND2 #(6) g33 (.I0(w31), .I1(w32), .Z(w28));   //: @(652,286) /sn:0 /w:[ 1 1 0 ]
  _GGBUF #(4) g49 (.I(w46), .Z(w48));   //: @(735,514) /sn:0 /w:[ 3 0 ]

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
  //: comment g2 @(9,10) /anc:1
  //: /line:"<h3>Editing Wires</h3> <b>(cutting)</b>"
  //: /line:""
  //: /line:"To cut a wire, select the <img src=\"cut_curs.gif\" bgcolor=gray> tool and click on a wire at the point you wish to cut.  Try"
  //: /line:"using the wire cutter tool to cut some of the wires in the circuit below."
  //: /end
  _GGNAND2 #(4) g1 (.I0(w7), .I1(w4), .Z(c1));   //: @(220,231) /sn:0 /R:3 /w:[ 1 1 0 ]
  //: comment g10 @(10,310) /sn:0 /R:14 /anc:1
  //: /line:"<tutorial-navigation>"
  //: /end
  _GGNAND2 #(4) g28 (.I0(a0), .I1(b0), .Z(w7));   //: @(192,208) /sn:0 /w:[ 5 5 0 ]
  //: joint g32 (w9) @(219, 152) /w:[ 1 -1 2 4 ]
  //: SWITCH g27 (c0) @(101,113) /sn:0 /w:[ 0 ] /st:0
  _GGNAND2 #(4) g31 (.I0(w9), .I1(c0), .Z(w4));   //: @(222,180) /sn:0 /R:3 /w:[ 5 5 0 ]
  //: joint g29 (a0) @(164, 149) /w:[ 2 -1 1 4 ]
  //: SWITCH g25 (a0) @(101,149) /sn:0 /w:[ 0 ] /st:0
  _GGXOR2 #(8) g24 (.I0(w9), .I1(c0), .Z(s0));   //: @(260,155) /sn:0 /w:[ 0 3 0 ]
  _GGXOR2 #(8) g23 (.I0(a0), .I1(b0), .Z(w9));   //: @(185,152) /sn:0 /w:[ 3 3 3 ]
  //: SWITCH g26 (b0) @(101,184) /sn:0 /w:[ 0 ] /st:0
  //: joint g33 (c0) @(224, 157) /w:[ 2 1 -1 4 ]
  //: joint g30 (b0) @(158, 184) /w:[ -1 2 1 4 ]

endmodule
//: /netlistEnd

//: /netlistBegin PAGE5
module PAGE5();
//: interface  /sz:(40, 40) /bd:[ ]
supply1 w7;    //: /sn:0 {0}(320,176)(335,176)(335,158){1}
reg [7:0] w3;    //: /sn:0 {0}(#:184,170)(184,208)(212,208)(212,224){1}
supply0 w8;    //: /sn:0 {0}(271,250)(271,238)(252,238){1}
supply0 w5;    //: /sn:0 {0}(339,207)(339,186)(320,186){1}
wire [7:0] w6;    //: /sn:0 {0}(281,171)(281,146)(371,146)(371,268)(#:228,268)(#:228,253){1}
wire w4;    //: /sn:0 {0}(204,238)(189,238){1}
wire [7:0] w1;    //: /sn:0 {0}(244,224)(244,211)(#:281,211)(#:281,192){1}
wire w2;    //: /sn:0 {0}(85,181)(244,181){1}
//: enddecls

  //: GROUND g4 (w5) @(339,213) /sn:0 /w:[ 0 ]
  //: DIP g8 (w3) @(184,160) /sn:0 /w:[ 0 ] /st:66
  //: comment g2 @(9,10) /anc:1
  //: /line:"<h3>Basic Editing Modes</h3> <b>(bit widths)</b>"
  //: /line:""
  //: /line:"To change the bit size of one or more wires, first enter the desired bit size in the"
  //: /line:"bit size selector <img src=\"size_example.gif\">.  Next, choose the <img src=\"size_curs.gif\" bgcolor=gray> tool, and click on each wire"
  //: /line:"to be changed to that size. Try changing the 8-bit wires in the circuit below to 16-bit wires."
  //: /line:""
  //: /end
  _GGREG8 #(10, 10, 20) g1 (.Q(w1), .D(w6), .EN(w5), .CLR(w7), .CK(w2));   //: @(281,181) /sn:0 /w:[ 1 0 1 0 1 ]
  //: comment g10 @(10,310) /sn:0 /R:14 /anc:1
  //: /line:"<tutorial-navigation>"
  //: /end
  _GGCLOCK_P100_0_50 g6 (.Z(w2));   //: @(72,181) /sn:0 /w:[ 0 ] /omega:100 /phi:0 /duty:50
  //: GROUND g7 (w8) @(271,256) /sn:0 /w:[ 0 ]
  //: VDD g5 (w7) @(346,158) /sn:0 /w:[ 1 ]
  _GGADD8 #(68, 70, 62, 64) g0 (.A(w3), .B(w1), .S(w6), .CI(w8), .CO(w4));   //: @(228,240) /sn:0 /w:[ 1 0 1 1 0 ]

endmodule
//: /netlistEnd

//: /netlistBegin PAGE4
module PAGE4();
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
  //: comment g2 @(9,10) /anc:1
  //: /line:"<h3>Basic Editing Modes</h3> <b>(I/O inverters)</b>"
  //: /line:""
  //: /line:"To place or remove an inverter on a gate input or output, select the <img src=\"inv_curs.gif\" bgcolor=gray> tool"
  //: /line:"and click on the port at which you want to place or remove an inveter. Try"
  //: /line:"using the inverter tool to place and remove inverters in the circuit below."
  //: /end
  _GGNAND2 #(4) g1 (.I0(w7), .I1(w4), .Z(c1));   //: @(220,231) /sn:0 /R:3 /w:[ 1 1 0 ]
  _GGNAND2 #(4) g28 (.I0(a0), .I1(b0), .Z(w7));   //: @(192,208) /sn:0 /w:[ 5 5 0 ]
  //: comment g10 @(10,310) /sn:0 /R:14 /anc:1
  //: /line:"<tutorial-navigation>"
  //: /end
  //: joint g32 (w9) @(219, 152) /w:[ 1 -1 2 4 ]
  //: SWITCH g27 (c0) @(101,113) /sn:0 /w:[ 0 ] /st:0
  _GGNAND2 #(4) g31 (.I0(w9), .I1(c0), .Z(w4));   //: @(222,180) /sn:0 /R:3 /w:[ 5 5 0 ]
  //: joint g29 (a0) @(164, 149) /w:[ 2 -1 1 4 ]
  //: SWITCH g25 (a0) @(101,149) /sn:0 /w:[ 0 ] /st:0
  _GGXOR2 #(8) g24 (.I0(w9), .I1(c0), .Z(s0));   //: @(260,155) /sn:0 /w:[ 0 3 0 ]
  _GGXOR2 #(8) g23 (.I0(a0), .I1(b0), .Z(w9));   //: @(185,152) /sn:0 /w:[ 3 3 3 ]
  //: SWITCH g26 (b0) @(101,184) /sn:0 /w:[ 0 ] /st:0
  //: joint g33 (c0) @(224, 157) /w:[ 2 1 -1 4 ]
  //: joint g30 (b0) @(158, 184) /w:[ -1 2 1 4 ]

endmodule
//: /netlistEnd

//: /netlistBegin PAGE2
module PAGE2();
//: interface  /sz:(40, 40) /bd:[ ]
//: enddecls

  //: comment g2 @(10,11)
  //: /line:"<h3>Basic Editing Modes</h3> <b>(properties)</b>"
  //: /line:""
  //: /line:"Wire properties"
  //: /line:""
  //: /end
  //: comment g0 @(10,310) /sn:0 /anc:1
  //: /line:"<tutorial-navigation>"
  //: /end

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
module _GGREG8 #(.Dsetup(1), .Dhold(1), .Dck_q(1)) (Q, D, EN, CLR, CK);
  input CK,EN,CLR;
  input  [7:0] D;
  output  [7:0] Q;
  reg 	  [7:0] Qreg;
 
  specify
      $setup(D,posedge CK, Dsetup);
      $hold(posedge CK,D, Dhold);
  endspecify

  assign #Dck_q Q = Qreg;

  always @(posedge CK or negedge CLR)
    if (CLR === 1'b0)
      Qreg = 8'b0;
    else if (CK === 1'b1 && EN === 1'b0)
      Qreg = D;

endmodule
//: /builtinEnd


//: /builtinBegin
module _GGBUF #(.Diz(1)) (I, Z);
   input  I;
   output  Z;

   assign #Diz Z = I;
   
endmodule // buf
//: /builtinEnd


//: /builtinBegin
module _GGADD8 #(.Dab_s(1), .Dab_co(1), .Dci_s(1), .Dci_co(1)) (A, B, S, CI, CO);
   input  CI;
   output CO;
   input   [7:0] A,B;
   output  [7:0] S;
   wire    [7:0] _S;
   wire   _CO;
   
   specify
      (A,B *> S) = Dab_s;
      (A,B *> CO) = Dab_co;
      (CI *> S) = Dci_s;
      (CI *> CO) = Dci_co;
   endspecify

   assign {_CO,_S} = A + B + CI;

   assign CO =  _CO;
   assign S =  _S;

endmodule
//: /builtinEnd


//: /builtinBegin
module _GGCLOCK_P100_0_50 (Z);
   output Z;
   reg 	  Z =  1'b0;

   initial #50
     forever
       begin
	  Z =  1'b1;
	  #50;
	  Z =  1'b0;
	  #50;
       end
   
endmodule // clock
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

