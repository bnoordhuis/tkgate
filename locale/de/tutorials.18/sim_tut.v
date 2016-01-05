//: version "1.6i"
//: property useExtBars = 0
//: property discardChanges = 1

module main;    //: root_module
wire w6;    //: /sn:0 {0}(750,326)(678,326){1}
wire w4;    //: /sn:0 {0}(652,260)(620,260){1}
//: {2}(618,258)(618,240)(588,240){3}
//: {4}(586,238)(586,227){5}
//: {6}(584,240)(571,240){7}
//: {8}(618,262)(618,323)(657,323){9}
wire w0;    //: /sn:0 {0}(750,321)(707,321)(707,305){1}
wire w3;    //: /sn:0 {0}(687,225)(707,225){1}
//: {2}(709,223)(709,214){3}
//: {4}(709,227)(709,256){5}
//: {6}(711,258)(749,258){7}
//: {8}(709,260)(709,284){9}
wire w1;    //: /sn:0 {0}(749,263)(706,263){1}
//: {2}(702,263)(673,263){3}
//: {4}(704,265)(704,284){5}
wire C;    //: /dp:1 {0}(798,312)(798,324)(771,324){1}
wire w5;    //: /sn:0 {0}(571,288)(585,288){1}
//: {2}(589,288)(607,288){3}
//: {4}(609,286)(609,265)(652,265){5}
//: {6}(609,290)(609,328)(657,328){7}
//: {8}(587,286)(587,276){9}
wire S;    //: {0}(770,261)(795,261)(795,251){1}
//: enddecls

  nand g8 (.I0(w0), .I1(w6), .Z(C));   //: @(761,324) /sn:0 /w:[ 0 0 1 ]
  xor g4 (.I0(w4), .I1(w5), .Z(w1));   //: @(663,263) /sn:0 /w:[ 0 5 3 ]
  //: comment g61 /dolink:1 /link:"@T/edwire_tut.v" @(630,100) /sn:0 /anc:1
  //: /line:"Leitungen editieren"
  //: /end
  //: switch g3 (w3) @(670,225) /sn:0 /w:[ 0 ] /st:1
  //: comment g13 /dolink:0 /link:"" @(25,45) /anc:1
  //: /line:"1) Drücke die \"Simulations\"-Taste aus der Knopfleiste oben oder "
  //: /line:"wähle \"Anfang Simulation\" aus dem \"Simulieren\"-Menu. Jetzt"
  //: /line:"sollte das Simulationsfenster erscheinen."
  //: /line:""
  //: /line:"2) Setze einen Tastkopf auf die zwei Leitungen mit dem Marken "
  //: /line:"\"S\" und \"C\" und auf jede Leitung, die von den Schaltern abgeht."
  //: /line:"(Mit einem Doppelklick!). Im Oszilloskop-Fenster erscheint für"
  //: /line:"jedes der Signale eine Zeitachse. "
  //: /line:""
  //: /line:"3) Drücke einigemale die Leertaste, um die Simulation "
  //: /line:"voranzubringen. Jeder Leertastendruck bringt die Simulationszeit"
  //: /line:"um eine Einheit, die sog. Epoche,voran. Das ist die kleinste"
  //: /line:"Zeiteinheit, die der Simulator auflösen kann."
  //: /line:""
  //: /line:"4) Wähle \"Optionen...\" aus dem \"Datei\"-Menu. Klicke auf die "
  //: /line:"Simulator-Taste im Optionen-Kasten und gib \"5\" als Eintrag"
  //: /line:"für die \"Epochen-Schrittweite\" ein, danach \"OK\". Wenn jetzt"
  //: /line:"die Leertaste gedrückt wird, schreitet der Simulator in "
  //: /line:"5-Epochen-Schritten voran."
  //: /line:""
  //: /line:"5) Klicke auf einen der Schalter, um den Wert zu ändern und"
  //: /line:"bewege den Simulator mit der Leertaste voran. Die richtigen"
  //: /line:"Simulationswerte sollten im Fenster erscheinen."
  //: /line:""
  //: /line:"6) Drücke und halte die linke Maustaste auf irgendeiner der "
  //: /line:"internen Verbindungen, um den aktuellen Wert anzusehen. Der "
  //: /line:"Wert wird im \"Verilog\"-Format angezeigt. Die erste Zahl gibt"
  //: /line:"die Bitbreite an, der Buchstabe die Basis und die Zahl am Schluß"
  //: /line:"den Wert. Zum Beispiel bedeutet \"1b0\", dass es ein 1-Bit-Wert"
  //: /line:"in Binärdarstellung mit Wert \"0\" ist."
  //: /line:""
  //: /line:"7) Drücke wieder die \"Simulations\"-Taste aus der Knopfleiste oder"
  //: /line:"wähle \"Run\" aus dem \"Simulieren\"-Menu. Das führt in den "
  //: /line:"kontinuierlichen Simulationsmodus. Versuche die "
  //: /line:"Schalterstellungen während der Simulation zu ändern. Nach jedem "
  //: /line:"Schalter-Stellungswechsel setzt sich die Simulation fort, bis die"
  //: /line:"Ausgänge stabil sind. Der \"Pause\"-Knopf läßt den Simulator in den"
  //: /line:"Einzelschrittmodus zurückkehren."
  //: /line:""
  //: /line:"8) Zur Beendigung der Simulation drückt man den \"Stop\"-Knopf auf der"
  //: /line:"Knopfleiste oder wählt \"Ende der Simulation\" aus dem \"Simulieren\"-"
  //: /line:"Menu aus."
  //: /end
  nand g2 (.I0(w4), .I1(w5), .Z(w6));   //: @(668,326) /sn:0 /w:[ 9 7 1 ]
  //: switch g1 (w5) @(554,288) /sn:0 /w:[ 0 ] /st:0
  //: joint g11 (w1) @(704, 263) /w:[ 1 -1 2 4 ]
  //: comment g16 /dolink:1 /link:"@T/module_tut.v" @(630,115) /sn:0 /anc:1
  //: /line:"Module benutzen"
  //: /end
  led g28 (.I(w4));   //: @(586,220) /sn:0 /w:[ 5 ] /type:0
  //: joint g10 (w3) @(709, 258) /w:[ 6 5 -1 8 ]
  //: joint g27 (w3) @(709, 225) /w:[ -1 2 1 4 ]
  //: joint g6 (w5) @(609, 288) /w:[ -1 4 3 6 ]
  nand g9 (.I0(w1), .I1(w3), .Z(w0));   //: @(707,295) /sn:0 /R:3 /w:[ 5 9 1 ]
  xor g7 (.I0(w3), .I1(w1), .Z(S));   //: @(760,261) /sn:0 /w:[ 7 0 0 ]
  //: joint g31 (w4) @(586, 240) /w:[ 3 4 6 -1 ]
  //: comment g20 /dolink:1 /link:"@T/create_tut.v" @(630,55) /sn:0 /anc:1
  //: /line:"Erzeugen einer Schaltung"
  //: /end
  //: frame g15 @(600,25) /sn:0 /anc:1 /wi:213 /ht:149 /tx:"Tutorien"
  led g29 (.I(w5));   //: @(587,269) /sn:0 /w:[ 9 ] /type:0
  led g25 (.I(C));   //: @(798,305) /sn:0 /w:[ 0 ] /type:0
  //: comment g17 /dolink:0 /link:"@T/sim_tut.v" @(630,130) /sn:0 /anc:1
  //: /line:"Schaltnetzsimulation"
  //: /end
  //: joint g5 (w4) @(618, 260) /w:[ 1 2 -1 8 ]
  //: comment g14 /dolink:1 /link:"@T/edit1_tut.v" @(630,70) /sn:0 /anc:1
  //: /line:"Grundlegende Editier-Modi"
  //: /end
  //: comment g21 /dolink:1 /link:"@T/seqsim_tut.v" @(630,145) /sn:0 /anc:1
  //: /line:"Schaltwerkssimulation"
  //: /end
  //: comment g24 /dolink:1 /link:"@T/welcome_tut.v" @(630,40) /sn:0 /anc:1
  //: /line:"TkGate Einführung"
  //: /end
  //: comment g23 /dolink:0 /link:"" @(605,130) /sn:0 /anc:1
  //: /line:"->"
  //: /end
  led g26 (.I(w3));   //: @(709,207) /sn:0 /w:[ 3 ] /type:0
  //: switch g0 (w4) @(554,240) /sn:0 /w:[ 7 ] /st:1
  //: comment g22 /dolink:1 /link:"@T/edit2_tut.v" @(630,85) /sn:0 /anc:1
  //: /line:"Editieren von Gruppen"
  //: /end
  led g18 (.I(S));   //: @(795,244) /sn:0 /w:[ 1 ] /type:0
  //: comment g12 /dolink:0 /link:"" @(16,18) /anc:1
  //: /line:"Dieses Tutorium führt in die Grundlagen der Simulation von Schaltnetzen ein."
  //: /end
  //: joint g30 (w5) @(587, 288) /w:[ 2 8 1 -1 ]

endmodule
