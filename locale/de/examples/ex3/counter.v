//: version "2.0-b6"
//: property encoding = "utf-8"
//: property locale = "de"
//: property prefix = "_GG"
//: property title = "counter.v"
//: property discardChanges = 1

`timescale 1ns/1ns

//: /netlistBegin main
module main;    //: root_module
reg [7:0] w4;    //: /sn:0 {0}(#:208,114)(208,167){1}
reg w8;    //: /sn:0 {0}(263,269)(325,269)(325,283){1}
supply0 w11;    //: /sn:0 {0}(269,197)(269,181)(248,181){1}
reg w9;    //: /sn:0 {0}(263,259)(324,259)(324,245){1}
wire w12;    //: /sn:0 {0}(200,181)(190,181){1}
wire ck;    //: {0}(50:187,264)(56,264){1}
wire [7:0] w5;    //: /sn:0 {0}(#:224,196)(224,254){1}
wire [7:0] reg_out;    //: {0}(#:50:224,275)(224,332)(297,332)(297,173){1}
//: {2}(299,171)(379,171){3}
//: {4}(297,169)(297,125)(240,125)(240,167){5}
//: enddecls

  //: comment g8 @(41,205)
  //: /line:"2) Setze per Doppelklick einen "
  //: /line:"Tastkopf auf die Taktleitung."
  //: /end
  _GGCLOCK_P100_0_50 g4 (.Z(ck));   //: @(43,264) /sn:0 /w:[ 1 ] /omega:100 /phi:0 /duty:50
  //: comment g13 @(23,412)
  //: /line:"7) Drücke die Tab-Taste, um einen Takt vorzurücken."
  //: /line:"Die Clear-Leitung ist nun gesetzt, das Register"
  //: /line:"wird zurückgesetzt.  Setze nun die Clear"
  //: /line:"Leitung auf \"on\" und fahre fort, die \"Tab\"-Taste"
  //: /line:"zu drücken.  Beobachte, wie der Registerwert mit jeder"
  //: /line:"Taktperiode hochzählt."
  //: /end
  //: DIP g3 (w4) @(208,104) /sn:0 /w:[ 0 ] /st:1
  _GGADD8 #(68, 70, 62, 64) g2 (.A(w4), .B(reg_out), .S(w5), .CI(w11), .CO(w12));   //: @(224,183) /sn:0 /w:[ 1 5 0 1 0 ]
  //: SWITCH g1 (w8) @(325,297) /sn:0 /R:1 /w:[ 1 ] /st:0
  //: comment g11 @(348,218)
  //: /line:"5) Stelle sicher, daß die Clear"
  //: /line:"Leitung CLR \"off\" ist, um das"
  //: /line:"Register zurückzusetzen."
  //: /end
  //: comment g10 @(270,64)
  //: /line:"4) Setze den DIP Schalter auf 1"
  //: /line:"(draufklicken, Eingeben einer"
  //: /line:"\"1\" in die Dialogbox die sich"
  //: /line:"öffnet) und drücke \"Anwenden\"."
  //: /end
  //: GROUND g6 (w11) @(269,203) /sn:0 /w:[ 0 ]
  //: comment g9 @(198,354)
  //: /line:"3) Setze einen Tastkopf auf den"
  //: /line:"Registerausgang durch"
  //: /line:"Doppelklick."
  //: /end
  //: SWITCH g7 (w9) @(324,232) /sn:0 /R:3 /w:[ 1 ] /st:0
  //: comment g39 @(20,12) /sn:0
  //: /line:"<a href=\"../index.v\">[ZURÜCK]</a>"
  //: /end
  //: LED g17 (reg_out) @(386,171) /sn:0 /R:3 /w:[ 3 ] /type:2
  //: joint g14 (reg_out) @(297, 171) /w:[ 2 4 -1 1 ]
  //: comment g5 @(14,33)
  //: /line:"8-Bit Zähler"
  //: /line:""
  //: /line:"1) Starte die Simulation durch Eingabe von"
  //: /line:"\"Ctl-S b\" auf der Tastatur"
  //: /line:""
  //: /end
  _GGREG8 #(10, 10, 20) g0 (.Q(reg_out), .D(w5), .EN(w8), .CLR(w9), .CK(ck));   //: @(224,264) /sn:0 /w:[ 0 1 0 0 0 ]
  //: comment g12 @(309,316)
  //: /line:"6) Stelle sicher, daß die Enable-Leitung"
  //: /line:"EN auf \"off\" steht."
  //: /end

endmodule
//: /netlistEnd


`timescale 1ns/1ns


//: /builtinBegin
module _GGREG8 #(.Dsetup(1), .Dhold(1), .Dck_q(1)) (Q, D, EN, CLR, CK);
  input CK,EN,CLR;
  input  [7:0] D;
  output  [7:0] Q;
  reg 	  [7:0] Qreg;
 
 // specify
   // $setup(D,posedge CK, Dsetup);
//    $hold(posedge CK,D, Dhold);
//  endspecify

  assign #Dck_q Q = Qreg;

  always @(posedge CK or negedge CLR)
    if (CLR === 1'b0)
      Qreg = 8'b0;
    else if (CK === 1'b1 && EN === 1'b0)
      Qreg = D;

endmodule
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

