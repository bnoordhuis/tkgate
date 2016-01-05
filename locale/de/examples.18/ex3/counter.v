//: version "1.6i"
//: property useExtBars = 0
//: property discardChanges = 1

module main;    //: root_module
supply0 w11;    //: /sn:0 {0}(269,197)(269,181)(248,181){1}
wire [7:0] w4;    //: /sn:0 {0}(208,114)(208,167){1}
wire w8;    //: /sn:0 {0}(263,269)(325,269)(325,283){1}
wire w12;    //: /sn:0 {0}(200,181)(190,181){1}
wire ck;    //: /dp:1 {0}(187,264)(56,264){1}
wire [7:0] w5;    //: /sn:0 {0}(224,196)(224,254){1}
wire [7:0] reg_out;    //: {0}(224,275)(224,332)(297,332)(297,173){1}
//: {2}(299,171)(379,171){3}
//: {4}(297,169)(297,125)(240,125)(240,167){5}
wire w9;    //: /sn:0 {0}(263,259)(324,259)(324,245){1}
//: enddecls

  //: comment g8 /dolink:0 /link:"" @(41,205)
  //: /line:"2) Setze einen Tastkopf"
  //: /line:"auf die Taktleitung"
  //: /line:"durch einen Doppelklick."
  //: /end
  clock g4 (.Z(ck));   //: @(43,264) /sn:0 /w:[ 1 ] /omega:100 /phi:0 /duty:50
  //: comment g13 /dolink:0 /link:"" @(23,412)
  //: /line:"7) Drücke nun die \"TAB\"-Taste, um eine Taktperiode"
  //: /line:"voranzuschreiten. Da die CLR-Leitung aktiv low ist,"
  //: /line:"wird das Register gelöscht. Setze dann die CLR-Leitung"
  //: /line:"auf \"on\" und drücke weiterhin die \"TAB\"-Taste."
  //: /line:"Beobachte den Wert des Registers, der mit jedem Takt"
  //: /line:"hochzählt."
  //: /end
  //: dip g3 (w4) @(208,104) /sn:0 /w:[ 0 ] /st:1
  add g2 (.A(w4), .B(reg_out), .S(w5), .CI(w11), .CO(w12));   //: @(224,183) /sn:0 /w:[ 1 5 0 1 0 ]
  //: switch g1 (w8) @(325,297) /sn:0 /R:1 /w:[ 1 ] /st:0
  //: comment g11 /dolink:0 /link:"" @(348,218)
  //: /line:"5) Die \"CLR\"-Leitung muß"
  //: /line:"ausgeschaltet sein, um das"
  //: /line:"Register zurückzusetzen."
  //: /end
  //: comment g10 /dolink:0 /link:"" @(270,64)
  //: /line:"4) Setze den Dip-Schalter mit einem"
  //: /line:"Klick auf 1, gib eine \"1\" in die"
  //: /line:"erscheinende Dialogbox ein und"
  //: /line:"drücke die \"Anwenden\"-Taste."
  //: /end
  //: supply0 g6 (w11) @(269,203) /sn:0 /w:[ 0 ]
  //: comment g9 /dolink:0 /link:"" @(198,354)
  //: /line:"3) Setze mit einem Doppelklick"
  //: /line:"einen Tastkopf auf den"
  //: /line:"Registerausgang."
  //: /end
  //: switch g7 (w9) @(324,232) /sn:0 /R:3 /w:[ 1 ] /st:0
  led g17 (.I(reg_out));   //: @(386,171) /sn:0 /R:3 /w:[ 3 ] /type:2
  //: joint g14 (reg_out) @(297, 171) /w:[ 2 4 -1 1 ]
  //: comment g5 /dolink:0 /link:"" @(15,11)
  //: /line:"Beispiel: 8-Bit Zähler"
  //: /line:""
  //: /line:"1) Starte die Simulation durch Eingabe von \"Strg-S b\"."
  //: /line:""
  //: /end
  register g0 (.Q(reg_out), .D(w5), .EN(w8), .CLR(w9), .CK(ck));   //: @(224,264) /sn:0 /w:[ 0 1 0 0 0 ]
  //: comment g12 /dolink:0 /link:"" @(309,316)
  //: /line:"6) Stelle sicher, daß die"
  //: /line:"Enable-Leitung EN ausgeschaltet ist."
  //: /end

endmodule
