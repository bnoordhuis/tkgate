//: version "1.7"
//: property useExtBars = 0
//: property discardChanges = 1

module main;    //: root_module
wire w3;    //: /sn:0 {0}(756,264)(679,264){1}
wire w1;    //: /sn:0 {0}(756,259)(712,259)(712,224)(679,224){1}
wire w2;    //: /sn:0 {0}(679,302)(689,302){1}
wire w5;    //: /sn:0 {0}(777,262)(787,262){1}
//: enddecls

  //: comment g61 /dolink:1 /link:"@T/edwire_tut.v" @(630,100) /sn:0 /anc:1
  //: /line:"Leitungen editieren"
  //: /end
  and g4 (.I0(w1), .I1(w3), .Z(w5));   //: @(767,262) /sn:0 /w:[ 0 0 0 ]
  //: switch g3 (w2) @(662,302) /sn:0 /w:[ 0 ] /st:0
  //: switch g2 (w3) @(662,264) /sn:0 /w:[ 1 ] /st:0
  //: switch g1 (w1) @(662,224) /sn:0 /w:[ 1 ] /st:0
  //: comment g16 /dolink:1 /link:"@T/module_tut.v" @(630,115) /sn:0 /anc:1
  //: /line:"Module benutzen"
  //: /end
  //: frame g11 @(434,355) /sn:0 /anc:1 /wi:373 /ht:230 /tx:"Gatter hier hin kopieren"
  //: frame g10 @(21,357) /sn:0 /anc:1 /wi:394 /ht:227 /tx:"Gatter hier hin ziehen"
  //: frame g15 @(600,25) /sn:0 /anc:1 /wi:213 /ht:149 /tx:"Tutorien"
  //: comment g20 /dolink:1 /link:"@T/create_tut.v" @(630,55) /sn:0 /anc:1
  //: /line:"Erzeugen einer Schaltung"
  //: /end
  //: comment g17 /dolink:1 /link:"@T/sim_tut.v" @(630,130) /sn:0 /anc:1
  //: /line:"Schaltnetzsimulation"
  //: /end
  //: comment g14 /dolink:1 /link:"@T/seqsim_tut.v" @(630,145) /sn:0 /anc:1
  //: /line:"Schaltwerkssimulation"
  //: /end
  //: comment g5 /dolink:0 /link:"" @(26,34) /anc:1
  //: /line:"1) Klicke mit der linken Maustaste links oberhalb der drei Schalter. Drücke"
  //: /line:"und halte die linke Maustaste und ziehe sie nach rechts unten. Nach ein paar"
  //: /line:"mm Ziehen erscheint ein Auswahlfenster. Bewege die Maus soweit, bis die "
  //: /line:"ganze Schaltung im Kasten eingeschlossen ist. Die Schalter und das Gatter sind"
  //: /line:"hervorgehoben, um die Selektion anzudeuten."
  //: /line:""
  //: /line:"2) Drücke und halte die Maus auf einem der ausgewählten Gatter. Ziehe die"
  //: /line:"ganze Gattergruppe in das Fenster \"Gatter hier hin ziehen\". Klicke irgendwo"
  //: /line:"auf die Canvas-Fläche, wo kein Gatter und kein Draht ist, um die Selektion "
  //: /line:"aufzuheben."
  //: /line:""
  //: /line:"3) Wähle das AND-Gatter in der Gattergruppe aus. Gehe ins \"Gatter\"-Menu und "
  //: /line:"wähle \"Eingang hinzufügen\" aus. Damit entsteht ein neuer Eingang des AND-"
  //: /line:"Gatters, der nun mit dem noch unbeschalteten Schalter verbunden werden kann."
  //: /line:""
  //: /line:"4) Nach den Anweisungen in 1) werden wieder die drei Schalter und das AND-Gatter"
  //: /line:"ausgewählt. Gehe nun ins \"Editieren\"-Menu und wähle \"Kopieren\" aus, um die "
  //: /line:"Gruppe der Gatter in den Ausschnittpuffer zu kopieren. Klicke nun mit der"
  //: /line:"linken Maustaste in die Mitte des Kastens \"Gatter hier hin kopieren\" und wähle "
  //: /line:"\"einfügen\"  aus dem \"Editieren\"-Menu. Das erzeugt eine Kopie des selektierten"
  //: /line:"Gatters. Man kann auch die \"Entf\"-Taste auf einer Gruppe selektierter Gatter"
  //: /line:"drücken, um sie zu entfernen."
  //: /line:""
  //: /line:"5) Eine Gruppenauswahl gelingt auch, wenn man auf ein Gatter klickt und die"
  //: /line:"\"Strg\"-Taste gedrückt hält. Das fügt ein Gatter nach dem anderen zu einer "
  //: /line:"Auswahl hinzu."
  //: /end
  //: comment g21 /dolink:0 /link:"@T/edit2_tut.v" @(630,85) /sn:0 /anc:1
  //: /line:"Editieren von Gruppen"
  //: /end
  //: comment g22 /dolink:0 /link:"" @(605,85) /sn:0 /anc:1
  //: /line:"->"
  //: /end
  //: comment g0 /dolink:0 /link:"" @(26,17) /anc:1
  //: /line:"Dieser Abschnitt führt in die Gruppen-Editiervorgänge ein."
  //: /end
  //: comment g18 /dolink:1 /link:"@T/welcome_tut.v" @(630,40) /sn:0 /anc:1
  //: /line:"TkGate Einführung"
  //: /end
  //: comment g12 /dolink:1 /link:"@T/edit1_tut.v" @(630,70) /sn:0 /anc:1
  //: /line:"Grundlegende Editier-Modi"
  //: /end

endmodule
