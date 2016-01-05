//: version "2.0-b6"
//: property encoding = "iso8859-1"
//: property locale = "en"
//: property prefix = "_GG"
//: property title = "module_tut.v"
//: property discardChanges = 1

`timescale 1ns/1ns

//: /netlistBegin PAGE1
module PAGE1;    //: root_module
//: enddecls

  //: comment g3 @(259,83) /anc:1
  //: /line:"A module is an encapsulated piece of logic that can be made"
  //: /line:"to serve a purpose in a circuit, similar to a function call"
  //: /line:"in a program.  Each module in TkGate has an \"interface\" and"
  //: /line:"a \"definition\":"
  //: /line:""
  //: /line:"  *  The <i color=green4>interface</i> describes the external"
  //: /line:"     appearance of a module including the size of the box and"
  //: /line:"     the positions of the ports on it.  TkGate supports \"block\""
  //: /line:"     interfaces and \"symbol\" interfaces."
  //: /line:"  "
  //: /line:"  *  The <i color=green4>definition</i> describes the internal"
  //: /line:"     logic that the module implements.  TkGate supports \"netlist\""
  //: /line:"     and \"HDL\" modules."
  //: /line:""
  //: /end
  //: comment g2 @(10,65) /sn:0 /anc:1
  //: /line:"<img src=bigmodule.gif>"
  //: /end
  //: comment g1 @(9,13) /anc:1
  //: /line:"<h3>Using Modules</h3>"
  //: /line:""
  //: /line:"In this chapter, you will learn how to create, edit and manage modules."
  //: /end
  //: comment g0 @(10,310) /sn:0 /R:14 /anc:1
  //: /line:"<tutorial-navigation>"
  //: /end

endmodule
//: /netlistEnd

//: /netlistBegin PAGE6
module PAGE6();
//: interface  /sz:(40, 40) /bd:[ ]
wire w4;    //: /sn:0 {0}(258,177)(273,177){1}
wire w3;    //: /sn:0 {0}(258,161)(273,161){1}
wire w0;    //: /sn:0 {0}(200,161)(185,161){1}
wire w1;    //: /sn:0 {0}(200,177)(185,177){1}
wire w2;    //: /sn:0 {0}(200,192)(185,192){1}
wire w5;    //: /sn:0 {0}(258,193)(273,193){1}
//: enddecls

  bar g4 (.C(w2), .B(w1), .A(w0), .Z(w5), .Y(w4), .X(w3));   //: @(201, 145) /sz:(56, 72) /sn:0 /p:[ Li0>0 Li1>0 Li2>0 Ro0<0 Ro1<0 Ro2<0 ]
  //: comment g3 @(10,310) /sn:0 /R:14
  //: /line:"<tutorial-navigation>"
  //: /end
  //: comment g2 @(10,310) /sn:0 /R:14
  //: /line:"<tutorial-navigation>"
  //: /end
  //: comment g1 @(9,13) /anc:1
  //: /line:"<h3>Using Modules</h3> <b>(editing module definitions)</b>"
  //: /line:""
  //: /line:"To edit the definition of a module you have created, double click on its name in the module name"
  //: /line:"tree or list view.  Alternatively you can right click on an instance of the module and select"
  //: /line:"<font color=red2>Open</font> from the popup menu."
  //: /end
  //: comment g0 @(10,310) /sn:0 /R:14
  //: /line:"<tutorial-navigation>"
  //: /end

endmodule
//: /netlistEnd

//: /netlistBegin PAGE3
module PAGE3();
//: interface  /sz:(40, 40) /bd:[ ]
//: enddecls

  //: comment g4 @(576,89) /sn:0 /anc:1
  //: /line:"<font color=green4>Hint: The pages in this tutorial"
  //: /line:"are actually modules with the"
  //: /line:"names PAGE1, PAGE2, etc.</font>"
  //: /end
  //: comment g3 @(23,72) /sn:0 /anc:1
  //: /line:"<img src=document.gif>   Top-level or root module."
  //: /line:""
  //: /line:"<img src=mod_net.gif>   Netlist module."
  //: /line:""
  //: /line:"<img src=mod_hdl.gif>   Verilog HDL module."
  //: /line:""
  //: /line:"<img src=mod_netL.gif>   Locked netlist module."
  //: /line:""
  //: /line:"<img src=mod_hdlL.gif>   Locked Verilog HDL module."
  //: /line:""
  //: /line:"<img src=unused.gif>   Container for unused modules."
  //: /line:""
  //: /end
  //: comment g2 @(257,69) /anc:1
  //: /line:"<img src=chip.gif>   Library module."
  //: /line:""
  //: /line:"<img src=parts.gif>   Container for all libraries."
  //: /line:""
  //: /line:"<img src=ichipdir.gif>   Container for a single library."
  //: /line:""
  //: /line:"<img src=unknown.gif>   Use of an undefined module."
  //: /line:""
  //: /line:"<img src=conflict.gif>   Recursive inclusion of a module."
  //: /line:""
  //: /end
  //: comment g1 @(9,13) /anc:1
  //: /line:"<h3>Using Modules</h3> <b>(module list symbols)</b>"
  //: /line:""
  //: /line:"The following symbols are used in the module tree and list view:"
  //: /line:""
  //: /end
  //: comment g0 @(10,310) /sn:0 /R:14 /anc:1
  //: /line:"<tutorial-navigation>"
  //: /end

endmodule
//: /netlistEnd

//: /netlistBegin foo
module foo(Z2, I3, Z1, I1, I2, Z3);
//: interface  /sz:(41, 40) /bd:[ ]
input I2;    //: /sn:0 {0}(245,200)(275,200){1}
//: {2}(277,198)(277,153)(292,153){3}
//: {4}(277,202)(277,248)(348,248){5}
output Z3;    //: /sn:0 {0}(369,251)(448,251){1}
input I1;    //: /sn:0 {0}(292,148)(278,148)(278,128)(246,128){1}
output Z2;    //: /sn:0 {0}(392,156)(380,156)(380,184){1}
//: {2}(382,186)(448,186){3}
//: {4}(378,186)(369,186){5}
output Z1;    //: /sn:0 {0}(448,154)(413,154){1}
input I3;    //: /sn:0 {0}(348,188)(296,188)(296,251){1}
//: {2}(298,253)(348,253){3}
//: {4}(294,253)(245,253){5}
wire w0;    //: /sn:0 {0}(348,183)(338,183)(338,153){1}
//: {2}(340,151)(392,151){3}
//: {4}(336,151)(313,151){5}
//: enddecls

  _GGAND2 #(6) g8 (.I0(I2), .I1(I3), .Z(Z3));   //: @(359,251) /sn:0 /w:[ 5 3 0 ] /eb:0
  //: OUT g4 (Z2) @(445,186) /sn:0 /w:[ 3 ]
  //: joint g13 (w0) @(338, 151) /w:[ 2 -1 4 1 ]
  //: IN g2 (I3) @(243,253) /sn:0 /w:[ 5 ]
  //: OUT g1 (Z1) @(445,154) /sn:0 /w:[ 0 ]
  //: comment g16 @(585,127) /sn:0
  //: /line:"<font color=green4>Hint: If you opened this module by"
  //: /line:"choosing <img src=blk_open.gif><font color=red2>Open</font> from the menu or"
  //: /line:"toolbar, you can close it by right"
  //: /line:"clicking in the canvas area and"
  //: /line:"selecting <font color=red2>Close</font>."
  //: /end
  _GGAND2 #(6) g11 (.I0(w0), .I1(Z2), .Z(Z1));   //: @(403,154) /sn:0 /w:[ 3 0 1 ] /eb:0
  //: joint g10 (I2) @(277, 200) /w:[ -1 2 1 4 ]
  //: IN g19 (I1) @(244,128) /sn:0 /w:[ 1 ]
  _GGXOR2 #(8) g6 (.I0(I1), .I1(I2), .Z(w0));   //: @(303,151) /sn:0 /w:[ 0 3 5 ] /eb:0
  //: joint g9 (I3) @(296, 253) /w:[ 2 1 4 -1 ]
  _GGXOR2 #(8) g7 (.I0(w0), .I1(I3), .Z(Z2));   //: @(359,186) /sn:0 /w:[ 0 0 5 ] /eb:0
  //: comment g15 @(22,25)
  //: /line:"This is the definition for module \"foo\"."
  //: /end
  //: comment g14 @(76,306) /sn:0
  //: /line:"<a href=\"#/PAGE8\">&lang;BACK TO TUTORIAL&rang;</a>"
  //: /end
  //: IN g5 (I2) @(243,200) /sn:0 /w:[ 0 ]
  //: joint g23 (Z2) @(380, 186) /w:[ 2 1 4 -1 ]
  //: OUT g12 (Z3) @(445,251) /sn:0 /w:[ 1 ]

endmodule
//: /netlistEnd

//: /netlistBegin ALU
module ALU(F, A, B, Z);
//: interface  /sz:(40, 40) /bd:[ ]
input [7:0] B;    //: /sn:0 {0}(#:342,147)(342,184){1}
//: {2}(344,186)(367,186){3}
//: {4}(371,186)(425,186)(425,258){5}
//: {6}(369,188)(369,261){7}
//: {8}(342,188)(342,258){9}
input [1:0] F;    //: /sn:0 {0}(#:343,353)(214,353){1}
supply0 w0;    //: /sn:0 {0}(491,295)(491,272)(465,272){1}
input [7:0] A;    //: /sn:0 {0}(#:265,147)(265,215){1}
//: {2}(267,217)(345,217){3}
//: {4}(349,217)(372,217){5}
//: {6}(376,217)(457,217)(457,258){7}
//: {8}(374,219)(374,261){9}
//: {10}(347,219)(347,258){11}
//: {12}(265,219)(265,322)(348,322)(348,337){13}
output [7:0] Z;    //: /sn:0 {0}(#:366,404)(366,366){1}
wire [7:0] w14;    //: /sn:0 {0}(#:345,279)(345,310)(360,310)(360,337){1}
wire w4;    //: /sn:0 {0}(417,272)(407,272){1}
wire [7:0] w11;    //: /sn:0 {0}(#:372,282)(372,337){1}
wire [7:0] w10;    //: /sn:0 {0}(#:384,337)(384,307)(441,307)(441,287){1}
//: enddecls

  _GGADD8 #(68, 70, 62, 64) g4 (.A(B), .B(A), .S(w10), .CI(w0), .CO(w4));   //: @(441,274) /sn:0 /w:[ 5 7 1 1 0 ]
  //: joint g8 (A) @(347, 217) /w:[ 4 -1 3 10 ]
  //: IN g3 (F) @(212,353) /sn:0 /w:[ 1 ]
  //: GROUND g13 (w0) @(491,301) /sn:0 /w:[ 0 ]
  //: IN g2 (A) @(265,145) /sn:0 /R:3 /w:[ 0 ]
  //: IN g1 (B) @(342,145) /sn:0 /R:3 /w:[ 0 ]
  //: joint g11 (B) @(369, 186) /w:[ 4 -1 3 6 ]
  //: joint g10 (A) @(374, 217) /w:[ 6 -1 5 8 ]
  _GGAND2x8 #(6) g6 (.I0(B), .I1(A), .Z(w11));   //: @(372,272) /sn:0 /R:3 /w:[ 7 9 0 ] /eb:0
  _GGOR2x8 #(6) g7 (.I0(B), .I1(A), .Z(w14));   //: @(345,269) /sn:0 /R:3 /w:[ 9 11 0 ] /eb:0
  //: joint g9 (A) @(265, 217) /w:[ 2 1 -1 12 ]
  //: OUT g5 (Z) @(366,401) /sn:0 /R:3 /w:[ 0 ]
  _GGMUX4x8 #(12, 12) g0 (.I0(A), .I1(w14), .I2(w11), .I3(w10), .S(F), .Z(Z));   //: @(366,353) /sn:0 /w:[ 13 1 1 0 0 1 ] /ss:0 /do:0
  //: joint g12 (B) @(342, 186) /w:[ 2 1 -1 8 ]

endmodule
//: /netlistEnd

//: /netlistBegin PAGE8
module PAGE8();
//: interface  /sz:(40, 40) /bd:[ ]
//: enddecls

  //: comment g3 @(573,160) /sn:0 /anc:1
  //: /line:"<font color=green4>Hint: If you are unable to modify a"
  //: /line:"module interface, right click on it,"
  //: /line:"select <img src=i_modprops.gif></font><font color=red2>Module Properties...</font><font color=green4>, then"
  //: /line:"unset the interface protection options"
  //: /line:"in the dialog box.</font>"
  //: /end
  foo g2 ();   //: @(208, 163) /sz:(40, 40) /sn:0 /p:[ ]
  //: comment g1 @(9,13) /anc:1
  //: /line:"<h3>Using Modules</h3> <b>(manual interface generation)</b>"
  //: /line:""
  //: /line:"You can also manually create and edit the interface of a module:"
  //: /line:" * To change the size of the interface, left-click on an edge or corner and drag it with the mouse."
  //: /line:" * To add a port, right click on an edge and choose the port type to create from the popup menu."
  //: /line:" * To delete a port, select the <img src=cut_curs.gif> tool and cut the wire off of the module."
  //: /line:" * To edit port properties, double click on the port with the <img src=mov_curs.gif> tool."
  //: /end
  //: comment g0 @(10,310) /sn:0 /R:14
  //: /line:"<tutorial-navigation>"
  //: /end

endmodule
//: /netlistEnd

//: /netlistBegin PAGE9
module PAGE9();
//: interface  /sz:(40, 40) /bd:[ ]
wire [1:0] w0;    //: /sn:0 {0}(#:257,226)(272,226){1}
wire [7:0] w3;    //: /sn:0 {0}(#:329,194)(314,194){1}
wire [7:0] w1;    //: /sn:0 {0}(#:257,210)(272,210){1}
wire [7:0] w2;    //: /sn:0 {0}(#:257,194)(272,194){1}
//: enddecls

  ALU g3 (.A(w2), .B(w1), .F(w0), .Z(w3));   //: @(273, 178) /sz:(40, 64) /sn:0 /p:[ Li0>1 Li1>1 Li2>1 Ro0<1 ]
  //: comment g2 @(10,310) /sn:0 /R:14
  //: /line:"<tutorial-navigation>"
  //: /end
  //: comment g1 @(9,13) /anc:1
  //: /line:"<h3>Using Modules</h3> <b>(updating the default interface)</b>"
  //: /line:""
  //: /line:"Module interfaces constructed in <img src=editmode.gif> <font color=red2>Edit</font> mode, apply only to the specific instance that you created."
  //: /line:"To make an interface the default for newly created instance of modules of that type, right click"
  //: /line:"on an interface and select <font color=red2>Interface &rarr;  Set</font>.  Try setting the interface to each of the two instances of"
  //: /line:"ALU modules below, then try creating new ALU modules by dragging them from the module list."
  //: /end
  ALU g0 ();   //: @(130, 184) /sz:(40, 40) /sn:0 /p:[ ]

endmodule
//: /netlistEnd

//: /netlistBegin PAGE7
module PAGE7();
//: interface  /sz:(40, 40) /bd:[ ]
//: enddecls

  //: comment g1 @(9,13) /anc:1
  //: /line:"<h3>Using Modules</h3> <b>(automatic interface generation)</b>"
  //: /line:""
  //: /line:"The interface of a module includes the size and shape of the box as well as the location and names"
  //: /line:"of the ports.  The easiest way to create an interface is to use automatic generation.  Automatic"
  //: /line:"generation uses the ports referenced in the definition to create the interface.  To automatically"
  //: /line:"create an interface for the \"bar\" module, first click on the module list tab <img src=modlist.gif bgcolor=gray>, then right click on \"bar\""
  //: /line:"and select <font color=red2>Interface &rarr;  Auto Generate...</font>  Now left click and drag the bar module onto the canvas to see"
  //: /line:"the new interface."
  //: /line:""
  //: /end
  //: comment g0 @(10,310) /sn:0 /R:14
  //: /line:"<tutorial-navigation>"
  //: /end

endmodule
//: /netlistEnd

//: /netlistBegin PAGE5
module PAGE5();
//: interface  /sz:(40, 40) /bd:[ ]
//: enddecls

  //: comment g1 @(9,13) /anc:1
  //: /line:"<h3>Using Modules</h3> <b>(creating module definitions)</b>"
  //: /line:""
  //: /line:"To create a new module, press the <img src=blk_new.gif> button.  Enter the name for the new module"
  //: /line:"and select whether you wish to create it as a \"netlist\" (graphical) module or an"
  //: /line:"HDL (textual) module.  Try creating a new module named \"CPU\"."
  //: /end
  //: comment g0 @(10,310) /sn:0 /R:14
  //: /line:"<tutorial-navigation>"
  //: /end

endmodule
//: /netlistEnd

//: /netlistBegin PAGE2
module PAGE2();
//: interface  /sz:(40, 40) /bd:[ ]
//: enddecls

  //: comment g3 @(548,6) /sn:0 /anc:1
  //: /line:"<img src=modlistfig.gif>"
  //: /end
  //: comment g2 @(554,346) /sn:0 /anc:1
  //: /line:"<font color=green4>Module Tree View Example</font>"
  //: /end
  //: comment g1 @(9,13) /anc:1
  //: /line:"<h3>Using Modules</h3> <b>(module list)</b>"
  //: /line:""
  //: /line:"The set of modules in the current circuit is shown on the left-hand side of the interface.  You"
  //: /line:"can choose between a <i>tree view</i> <img src=modtree.gif bgcolor=gray> (see example) and a <i>list view</i> <img src=modlist.gif bgcolor=gray>.  The tree view shows"
  //: /line:"the module hierarchy starting at a top-level root module indicated by the <img src=document.gif> symbol.  Modules"
  //: /line:"which are still in the circuit, but are not used anywhere in the hierarchy are listed under the"
  //: /line:"\"<img src=unused.gif> Unused\" branch.  Click on <img src=hboxopen.gif> to open a branch on the tree."
  //: /line:""
  //: /end
  //: comment g0 @(10,310) /sn:0 /R:14 /anc:1
  //: /line:"<tutorial-navigation>"
  //: /end

endmodule
//: /netlistEnd

//: /netlistBegin PAGE4
module PAGE4();
//: interface  /sz:(40, 40) /bd:[ ]
//: enddecls

  //: comment g2 @(545,102) /sn:0 /anc:1
  //: /line:"<font color=green4>Hint: You can also drag and drop"
  //: /line:"module instance from the tree view,"
  //: /line:"but it is easier to use the list view"
  //: /line:"since that view gives you a straight"
  //: /line:"alphabetical list of all module.</font>"
  //: /end
  //: comment g1 @(9,13) /anc:1
  //: /line:"<h3>Using Modules</h3> <b>(creating module instances)</b>"
  //: /line:""
  //: /line:"The easiest way to create a module instance is to find its name in the module list and"
  //: /line:"drag it into the current circuit.  Try switching to the list view by pressing <img src=modlist.gif> then"
  //: /line:"dragging several instances of the \"bar\" module into the current edit window."
  //: /end
  //: comment g0 @(10,310) /sn:0 /R:14 /anc:1
  //: /line:"<tutorial-navigation>"
  //: /end

endmodule
//: /netlistEnd

//: /netlistBegin PAGE10
module PAGE10();
//: interface  /sz:(40, 40) /bd:[ ]
//: enddecls

  //: comment g2 @(10,310) /sn:0 /R:14
  //: /line:"<tutorial-navigation>"
  //: /end
  //: comment g1 @(11,22) /anc:1
  //: /line:"<h3>Using Modules</h3> <b>(interface editor)</b>"
  //: /line:""
  //: /line:"You can directly edit the module interface using TkGate's interface mode.  Click on the <img src=editintr.gif> <font color=red2>Interface</font>"
  //: /line:"tab over the main canvas to enter this mode.  Details on how to use this mode are given in the TkGate"
  //: /line:"documentation.  Click on the <img src=editmode.gif> <font color=red2>Edit</font> tab to return to the normal editing mode."
  //: /end

endmodule
//: /netlistEnd

//: /netlistBegin bar
module bar(Y, X, C, B, Z, A);
//: interface  /sz:(56, 72) /bd:[ Li0>A(16/72) Li1>B(32/72) Li2>C(48/72) Ro0<X(16/72) Ro1<Y(32/72) Ro2<Z(48/72) ]
//: property pptype=0
output X;    //: /sn:0 {0}(346,161)(268,161){1}
//: {2}(266,159)(266,131)(282,131){3}
//: {4}(264,161)(254,161){5}
input B;    //: /sn:0 {0}(86,207)(116,207){1}
//: {2}(118,205)(118,160)(133,160){3}
//: {4}(118,209)(118,255)(189,255){5}
input A;    //: /sn:0 {0}(133,155)(50:118,155)(118,135)(86,135){1}
output Z;    //: /sn:0 {0}(210,258)(346,258){1}
input C;    //: /sn:0 {0}(189,195)(137,195)(137,258){1}
//: {2}(139,260)(160,260){3}
//: {4}(164,260)(189,260){5}
//: {6}(162,258)(162,126)(282,126){7}
//: {8}(135,260)(86,260){9}
output Y;    //: /sn:0 {0}(233,163)(221,163)(221,191){1}
//: {2}(223,193)(346,193){3}
//: {4}(219,193)(210,193){5}
wire w3;    //: /sn:0 {0}(303,129)(318,129){1}
wire w0;    //: /sn:0 {0}(189,190)(179,190)(179,160){1}
//: {2}(181,158)(233,158){3}
//: {4}(177,158)(154,158){5}
//: enddecls

  _GGAND2 #(6) g8 (.I0(B), .I1(C), .Z(Z));   //: @(200,258) /sn:0 /w:[ 5 5 0 ] /eb:0
  //: OUT g4 (Y) @(343,193) /sn:0 /w:[ 3 ]
  //: OUT g3 (X) @(343,161) /sn:0 /w:[ 0 ]
  //: joint g13 (w0) @(179, 158) /w:[ 2 -1 4 1 ]
  //: IN g2 (C) @(84,260) /sn:0 /w:[ 9 ]
  //: IN g1 (B) @(84,207) /sn:0 /w:[ 0 ]
  _GGAND2 #(6) g11 (.I0(w0), .I1(Y), .Z(X));   //: @(244,161) /sn:0 /w:[ 3 0 5 ] /eb:0
  //: comment g16 @(585,127) /sn:0 /anc:1
  //: /line:"<font color=green4>Hint: If you opened this module by"
  //: /line:"choosing <img src=blk_open.gif><font color=red2>Open</font> from the menu or"
  //: /line:"toolbar, you can close it by right"
  //: /line:"clicking in the canvas area and"
  //: /line:"selecting <font color=red2>Close</font>."
  //: /end
  //: joint g10 (B) @(118, 207) /w:[ -1 2 1 4 ]
  //: joint g19 (C) @(162, 260) /w:[ 4 6 3 -1 ]
  _GGXOR2 #(8) g6 (.I0(A), .I1(B), .Z(w0));   //: @(144,158) /sn:0 /w:[ 0 3 5 ] /eb:0
  //: joint g9 (C) @(137, 260) /w:[ 2 1 8 -1 ]
  _GGXOR2 #(8) g7 (.I0(w0), .I1(C), .Z(Y));   //: @(200,193) /sn:0 /w:[ 0 0 5 ] /eb:0
  //: comment g15 @(22,25)
  //: /line:"This is the definition for module \"bar\". Ports A, B and C are inputs, and ports X, Y and Z are"
  //: /line:"outputs.  Try adding an additional output port \"W\" from the output of the XOR gate below."
  //: /line:"Right click at the position you want to create the port then select <font color=red2>Make &rarr;   Module &rarr;   Module"
  //: /line:"Output</font> from the popup menu.  Enter \"W\" for the net name in the dialog box."
  //: /line:""
  //: /line:""
  //: /end
  _GGXOR2 #(8) g17 (.I0(C), .I1(X), .Z(w3));   //: @(293,129) /sn:0 /w:[ 7 3 0 ]
  //: OUT g5 (Z) @(343,258) /sn:0 /w:[ 1 ]
  //: comment g14 @(76,306) /sn:0
  //: /line:"<a href=\"#/PAGE6\">&lang;BACK TO TUTORIAL&rang;</a>"
  //: /end
  //: IN g0 (A) @(84,135) /sn:0 /w:[ 1 ]
  //: joint g18 (X) @(266, 161) /w:[ 1 2 4 -1 ]
  //: joint g12 (Y) @(221, 193) /w:[ 2 1 4 -1 ]

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
module _GGMUX4x8 #(.Dsz(1), .Diz(1)) (I0, I1, I2, I3, S, Z);
   input  [7:0] I0, I1, I2, I3;
   input  [1:0] S;
   output  [7:0] Z;
   reg 	   [7:0] Z;

   always
     begin
	case (S)
	  2'd0: Z <= #Dsz  I0;
	  2'd1: Z <= #Dsz  I1;
	  2'd2: Z <= #Dsz  I2;
	  2'd3: Z <= #Dsz  I3;
	  default: Z <= #Dsz  8'hx;
	endcase // case(S)
	@(S or I0 or I1 or I2 or I3);
     end
      
endmodule // mux
//: /builtinEnd


//: /builtinBegin
module _GGOR2x8 #(.Diz(1)) (I0, I1,  Z);
   output  [7:0] Z;
   input   [7:0] I0, I1;

     assign #Diz Z = ( I0 | I1 );
   
endmodule // or
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
module _GGAND2x8 #(.Diz(1)) (I0, I1,  Z);
   output  [7:0] Z;
   input   [7:0] I0, I1;
   reg  [7:0] Z;

   assign #Diz Z = ( I0 & I1 );
   
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

