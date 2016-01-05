//: version "1.6i"
//: property useExtBars = 0
//: property discardChanges = 1

module main;    //: root_module
supply0 w3;    //: /sn:0 {0}(705,275)(705,265)(692,265){1}
supply0 w0;    //: /sn:0 /dp:1 {0}(707,320)(730,320)(730,335){1}
wire [7:0] w7;    //: /sn:0 {0}(652,225)(652,251){1}
wire w4;    //: /sn:0 /dp:1 {0}(707,310)(718,310)(718,298)(728,298){1}
wire [7:0] w2;    //: /sn:0 /dp:1 {0}(668,305)(668,280){1}
wire ck;    //: /dp:1 {0}(631,315)(566,315){1}
wire [7:0] reg_out;    //: {0}(668,326)(668,368)(724,368){1}
//: {2}(728,368)(784,368)(784,241)(684,241)(684,251){3}
//: {4}(726,370)(726,400){5}
wire w9;    //: /sn:0 {0}(644,265)(634,265){1}
//: enddecls

  //: joint g8 (reg_out) @(726, 368) /w:[ 2 -1 1 4 ]
  //: supply0 g4 (w0) @(730,341) /sn:0 /w:[ 1 ]
  //: comment g61 /dolink:1 /link:"@T/edwire_tut.v" @(630,100) /sn:0 /anc:1
  //: /line:"Leitungen editieren"
  //: /end
  //: switch g3 (w4) @(746,298) /sn:0 /R:2 /w:[ 1 ] /st:1
  //: comment g13 /dolink:0 /link:"" @(24,45) /anc:1
  //: /line:"1) Drücke den \"Simulationsknopf\" in der Knopfleiste oben oder wähle"
  //: /line:"\"Anfang Simulation\" aus dem \"Simulieren\"-Menu. Das Simulationsfenster"
  //: /line:"sollte erscheinen."
  //: /line:""
  //: /line:"2) Bringe je einen Tastkopf auf der mit \"ck\" markierten Leitung"
  //: /line:"und auf den mit \"reg_out\" markierten Ausgangsleitungen des Registers"
  //: /line:"mit einem Doppelklick an. Für jedes Signal sollte ein Signaldiagramm"
  //: /line:"im Fenster erscheinen."
  //: /line:""
  //: /line:"3) Die Simulation schreitet fort, wenn man mehrfach nacheinander"
  //: /line:"die Leertaste drückt. Eine Epoche ist die kleinste vom Simulator "
  //: /line:"modellierte Zeiteinheit. Die Einzelheiten zum Setzen der Schrittweite "
  //: /line:"stehen im Tutorium für Schaltnetze."
  //: /line:""
  //: /line:"4) Simulationsfortschritt um einen Taktzyklus wird durch Drücken"
  //: /line:"der \"TAB\"-Taste oder durch \"Periodenschritt\" im \"Simulieren\"-Menu"
  //: /line:"erreicht. Die Simulation schreitet bis zur nächsten positiven Flanke "
  //: /line:"in jedem Taktgenerator der Schaltung fort und zusätzlich noch einige"
  //: /line:"Epochenschritte, um den Registern Zeit zu geben, die neuen Werte vor"
  //: /line:"dem Pausieren der Simulation einzuspeichern. Man kan diesen Überschuß"
  //: /line:"ebenso wie die Anzahl der fortzuschreitenden Taktzyklen ändern oder"
  //: /line:"Einzeltakt wählen über die Simulationsoptionen in der Optionen-"
  //: /line:"Dialogbox."
  //: /line:""
  //: /line:"5) Setze in diesem Beispiel den Schalter CLR auf \"off\" und simuliere"
  //: /line:"einen Takt, um das Register zu löschen. Schalte dann den Schalter auf"
  //: /line:"\"on\" und treibe die Simulation ein paar Takte voran, um den Zähler zu"
  //: /line:"erhöhen."
  //: /line:""
  //: /line:"6) Klicke auf den DIP-Schalter. Es öffnet sich ein Fenster, in dem man"
  //: /line:"den DIP-Schalter-wert ändern kann. Gib eine Zahl (in HEX) ein und drücke"
  //: /line:"\"Anwenden\", um die Dauer des Intervallschritts zu ändern."
  //: /line:""
  //: /line:"7) Genau wie im Schaltnetzbeispiel kann man hier auch auf Leitungen klicken,"
  //: /line:"um den (logischen) Wert auf dem Draht zu sehen."
  //: /line:""
  //: /line:"8) Drücke den \"Simulationsknopf\" in der Knopfleiste erneut oder wähle "
  //: /line:"\"Run\" aus dem \"Simulieren\"-Menu. Dann geht der Simulator in den"
  //: /line:"kontinuierlichen Simulationsmodus, bis der \"Pause\"-Knopf gedrückt wird."
  //: /line:"Man kann weiterhin die Werte von Schaltern etc. ändern, während"
  //: /line:"die Simulation läuft."
  //: /line:""
  //: /end
  register g2 (.Q(reg_out), .D(w2), .EN(w0), .CLR(w4), .CK(ck));   //: @(668,315) /sn:0 /w:[ 0 0 0 0 0 ]
  //: comment g1 /dolink:1 /link:"@T/edit1_tut.v" @(630,70) /sn:0 /anc:1
  //: /line:"Grundlegende Editier-Modi"
  //: /end
  //: comment g16 /dolink:1 /link:"@T/module_tut.v" @(630,115) /sn:0 /anc:1
  //: /line:"Module benutzen"
  //: /end
  //: comment g10 /dolink:0 /link:"" @(531,478) /sn:0 /R:2
  //: /line:"9) Man kann das Oszilloskopbild entweder"
  //: /line:"mit dem Rollbalken verschieben oder in"
  //: /line:"das Diagrammfenster klicken und die Spuren"
  //: /line:"nach links oder rechts verschieben. Zoom"
  //: /line:"In und Out ist auch möglich: Klicke mit"
  //: /line:"gedrückter Strg- oder Shift-Taste."
  //: /line:"Das Ziehen des Rollbalkens ganz nach rechts"
  //: /line:"führt dazu, dass die aktuellen Werte erneut"
  //: /line:"kontinuierlich aufgefrischt werden."
  //: /end
  //: dip g6 (w7) @(652,215) /sn:0 /w:[ 0 ] /st:1
  //: supply0 g9 (w3) @(705,281) /sn:0 /w:[ 0 ]
  led g7 (.I(reg_out));   //: @(726,407) /sn:0 /R:2 /w:[ 5 ] /type:2
  //: comment g20 /dolink:1 /link:"@T/create_tut.v" @(630,55) /sn:0 /anc:1
  //: /line:"Schaltung erzeugen"
  //: /end
  //: frame g15 @(600,25) /sn:0 /anc:1 /wi:213 /ht:149 /tx:"Tutorien"
  //: comment g17 /dolink:1 /link:"@T/sim_tut.v" @(630,130) /sn:0 /anc:1
  //: /line:"Schaltnetzsimulation"
  //: /end
  add g5 (.A(w7), .B(reg_out), .S(w2), .CI(w3), .CO(w9));   //: @(668,267) /sn:0 /w:[ 1 3 1 1 0 ]
  //: comment g14 /dolink:0 /link:"@T/seqsim_tut.v" @(630,145) /sn:0 /anc:1
  //: /line:"Schaltwerkssimulation"
  //: /end
  //: comment g21 /dolink:1 /link:"@T/edit2_tut.v" @(630,85) /sn:0 /anc:1
  //: /line:"Editiiern von Gruppen"
  //: /end
  clock g0 (.Z(ck));   //: @(553,315) /sn:0 /w:[ 1 ] /omega:100 /phi:0 /duty:50
  //: comment g22 /dolink:0 /link:"" @(605,145) /sn:0 /anc:1
  //: /line:"->"
  //: /end
  //: comment g12 /dolink:0 /link:"" @(16,18) /anc:1
  //: /line:"Dieses Tutorium führt in die Grundlagen der Simulation von Schaltwerken ein."
  //: /line:""
  //: /end
  //: comment g18 /dolink:1 /link:"@T/welcome_tut.v" @(630,40) /sn:0 /anc:1
  //: /line:"TkGate Einführung"
  //: /end

endmodule
