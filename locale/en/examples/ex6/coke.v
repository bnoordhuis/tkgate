//: version "2.0-b6"
//: property encoding = "iso8859-1"
//: property locale = "en"
//: property prefix = "_GG"
//: property title = "Welcome to TkGate Page"
//: require "tty"
//: require "coke"

`timescale 1ns/1ns

//: /netlistBegin main
module main;    //: root_module
reg [5:0] w6;    //: /sn:0 {0}(#:95,107)(114,107)(114,117)(143,117){1}
reg BILLT;    //: /sn:0 {0}(143,213)(49,213){1}
reg [7:0] w0;    //: /sn:0 {0}(#:88,321)(127,321)(127,293)(143,293){1}
reg RETINS;    //: /sn:0 {0}(143,261)(88,261){1}
reg CHGD;    //: /sn:0 {0}(143,149)(49,149){1}
reg [5:0] w20;    //: /sn:0 {0}(#:95,68)(124,68)(124,101)(143,101){1}
reg INSACK;    //: /sn:0 {0}(143,277)(49,277){1}
reg w1;    //: /sn:0 {0}(236,375)(236,358){1}
reg BILLIN;    //: /sn:0 {0}(143,197)(88,197){1}
reg BILLOUT;    //: /sn:0 {0}(143,229)(88,229){1}
reg CHGQ;    //: /sn:0 {0}(143,133)(88,133){1}
reg NOCHG;    //: /sn:0 {0}(143,181)(49,181){1}
reg CHGCOM;    //: /sn:0 {0}(143,245)(49,245){1}
reg CHGN;    //: /sn:0 {0}(143,165)(88,165){1}
wire w38;    //: /sn:0 {0}(326,325)(358,325){1}
wire w37;    //: /sn:0 {0}(326,309)(358,309){1}
wire w34;    //: /sn:0 {0}(326,261)(358,261){1}
wire [2:0] w21;    //: /sn:0 {0}(#:358,197)(326,197){1}
wire w31;    //: /sn:0 {0}(326,245)(358,245){1}
wire w36;    //: /sn:0 {0}(326,293)(358,293){1}
wire w24;    //: /sn:0 {0}(326,149)(358,149){1}
wire w23;    //: /sn:0 {0}(326,133)(358,133){1}
wire [5:0] w18;    //: /sn:0 {0}(#:326,117)(354,117){1}
wire w30;    //: /sn:0 {0}(326,229)(358,229){1}
wire INSN;    //: /sn:0 {0}(358,165)(326,165){1}
wire [5:0] w17;    //: /sn:0 {0}(#:326,101)(354,101){1}
wire [2:0] w29;    //: /sn:0 {0}(#:326,213)(358,213){1}
wire [2:0] w26;    //: /sn:0 {0}(#:326,181)(358,181){1}
wire BILLACK;    //: /sn:0 {0}(358,277)(326,277){1}
//: enddecls

  //: SWITCH g8 (BILLOUT) @(71,229) /sn:0 /w:[ 1 ] /st:0
  //: SWITCH g4 (CHGN) @(71,165) /sn:0 /w:[ 1 ] /st:0
  //: LED g13 (INSN) @(365,165) /sn:0 /R:3 /w:[ 0 ] /type:0
  //: SWITCH g3 (CHGD) @(32,149) /sn:0 /w:[ 1 ] /st:0
  //: SWITCH g2 (CHGQ) @(71,133) /sn:0 /w:[ 1 ] /st:0
  //: DIP g1 (w20) @(57,68) /sn:0 /R:1 /w:[ 0 ] /st:0
  //: DIP g11 (w6) @(57,107) /sn:0 /R:1 /w:[ 0 ] /st:0
  //: LED g16 (w21) @(365,197) /sn:0 /R:3 /w:[ 0 ] /type:1
  //: LED g28 (w37) @(365,309) /sn:0 /R:3 /w:[ 1 ] /type:0
  //: LED g10 (w23) @(365,133) /sn:0 /R:3 /w:[ 1 ] /type:0
  //: SWITCH g27 (INSACK) @(32,277) /sn:0 /w:[ 1 ] /st:0
  //: LED g19 (w31) @(365,245) /sn:0 /R:3 /w:[ 1 ] /type:0
  //: SWITCH g6 (BILLIN) @(71,197) /sn:0 /w:[ 1 ] /st:0
  //: LED g9 (w17) @(361,101) /sn:0 /R:3 /w:[ 1 ] /type:1
  //: SWITCH g7 (BILLT) @(32,213) /sn:0 /w:[ 1 ] /st:0
  //: comment g31 @(20,16) /sn:0
  //: /line:"<h1 color=red>Coke Machine Problem</h1>"
  //: /end
  //: LED g20 (w34) @(365,261) /sn:0 /R:3 /w:[ 1 ] /type:0
  //: LED g15 (w18) @(361,117) /sn:0 /R:3 /w:[ 1 ] /type:1
  //: comment g39 @(18,426) /sn:0
  //: /line:"<a href=\"../index.v\">[BACK]</a>"
  //: /end
  //: LED g29 (w38) @(365,325) /sn:0 /R:3 /w:[ 1 ] /type:0
  //: LED g25 (BILLACK) @(365,277) /sn:0 /R:3 /w:[ 0 ] /type:0
  //: LED g17 (w29) @(365,213) /sn:0 /R:3 /w:[ 1 ] /type:1
  //: LED g14 (w26) @(365,181) /sn:0 /R:3 /w:[ 1 ] /type:1
  //: SWITCH g5 (NOCHG) @(32,181) /sn:0 /w:[ 1 ] /st:0
  cokemachine g21 (.COST(w0), .INSACK(INSACK), .RETINS(RETINS), .CHGCOM(CHGCOM), .BILLOUT(BILLOUT), .BILLLT(BILLT), .BILLIN(BILLIN), .NOCHG(NOCHG), .CHGN(CHGN), .CHGD(CHGD), .CHGQ(CHGQ), .ELIGHT(w6), .DISP(w20), ._RESET(w1), .BILLNG(w38), .DISPACK(w37), .CHGACK(w36), .BILLACK(BILLACK), .CNRET(w34), .BILLOK(w31), .BILLSNS(w30), .NUMN(w29), .NUMD(w21), .NUMQ(w26), .INSN(INSN), .INSD(w24), .INSQ(w23), .EMPTY(w18), .PRESS(w17));   //: @(144, 85) /sz:(181, 272) /sn:0 /p:[ Li0>1 Li1>0 Li2>0 Li3>0 Li4>0 Li5>0 Li6>0 Li7>0 Li8>0 Li9>0 Li10>0 Li11>1 Li12>1 Bi0>1 Ro0<0 Ro1<0 Ro2<0 Ro3<1 Ro4<0 Ro5<0 Ro6<0 Ro7<0 Ro8<1 Ro9<0 Ro10<1 Ro11<0 Ro12<0 Ro13<0 Ro14<0 ]
  //: LED g24 (w36) @(365,293) /sn:0 /R:3 /w:[ 1 ] /type:0
  //: SWITCH g23 (RETINS) @(71,261) /sn:0 /w:[ 1 ] /st:0
  //: DIP g26 (w0) @(50,321) /sn:0 /R:1 /w:[ 0 ] /st:15
  //: comment g0 @(421,39) /sn:0
  //: /line:"The module to the left is an interface to a \"coke\" machine."
  //: /line:"If you switch to simulation mode and press the simulation"
  //: /line:"\"play\" button, a window showing an external and internal"
  //: /line:"view of the coke machine will be displayed.  You can then"
  //: /line:"drag coins and bills to the coin slot or bill reader, press"
  //: /line:"the drink buttons or press the coin return lever. These"
  //: /line:"actions will cause signals to be sent to your circuit."
  //: /line:"Inputs to the controller can be used to illuminate lights"
  //: /line:"and/or dispense drinks and change."
  //: /line:""
  //: /line:"Problems:"
  //: /line:""
  //: /line:"1) Design a controler to control the coke machine via the"
  //: /line:"interface above."
  //: /line:""
  //: /line:"2) Modify your design so that either of the two \"Coke\""
  //: /line:"buttons can always be used when there is any Coke in the"
  //: /line:"machine."
  //: /line:""
  //: /end
  //: SWITCH g22 (CHGCOM) @(32,245) /sn:0 /w:[ 1 ] /st:0
  //: LED g18 (w30) @(365,229) /sn:0 /R:3 /w:[ 1 ] /type:0
  //: LED g12 (w24) @(365,149) /sn:0 /R:3 /w:[ 1 ] /type:0
  //: SWITCH g30 (w1) @(236,389) /sn:0 /R:1 /w:[ 0 ] /st:1

endmodule
//: /netlistEnd


`timescale 1ns/1ns

