//: version "1.6i"
//: property useExtBars = 0
//: property discardChanges = 1

module main;    //: root_module
wire w6;    //: /sn:0 {0}(232,502)(177,502){1}
//: {2}(175,500)(175,466)(187,466){3}
//: {4}(173,502)(141,502){5}
wire w7;    //: /sn:0 {0}(253,500)(263,500){1}
wire w2;    //: /sn:0 {0}(187,461)(141,461){1}
wire w5;    //: /sn:0 {0}(232,497)(218,497)(218,464)(208,464){1}
//: enddecls

  or g4 (.I0(w5), .I1(w6), .Z(w7));   //: @(243,500) /sn:0 /anc:1 /w:[ 0 0 0 ]
  //: comment g8 /dolink:0 /link:"" @(250,481) /anc:1
  //: /line:"OR-Gatter"
  //: /end
  //: comment g61 /dolink:1 /link:"@T/edwire_tut.v" @(630,100) /sn:0 /anc:1
  //: /line:"Leitungen editeren"
  //: /end
  //: frame g13 @(410,406) /sn:0 /wi:381 /ht:177 /tx:"Erzeuge die Schaltung hier"
  and g3 (.I0(w2), .I1(w6), .Z(w5));   //: @(198,464) /sn:0 /anc:1 /w:[ 0 3 1 ]
  //: switch g2 (w6) @(124,502) /sn:0 /anc:1 /w:[ 5 ] /st:0
  //: switch g1 (w2) @(124,461) /sn:0 /anc:1 /w:[ 1 ] /st:0
  //: comment g16 /dolink:1 /link:"@T/module_tut.v" @(630,115) /sn:0 /anc:1
  //: /line:"Module benutzen"
  //: /end
  //: comment g10 /dolink:0 /link:"" @(12,236) /anc:1
  //: /line:"5) Erzeuge zwei Schalter auf gleichem Wege wie die AND und OR-Gatter durch"
  //: /line:"Setzen einer Position und Auswahl von \"Schalter\" aus dem \"I/O\"-Submenu des"
  //: /line:"\"Make\"-Menu oder mit dem Tastaturkürzel \"s\"."
  //: /line:""
  //: /line:"4) wiederhole Schritt 4, um den Ausgang des oberen Schalters mit dem oberen"
  //: /line:"Eingang des AND-Gatters zu verbinden."
  //: /line:""
  //: /line:"7) Wiederhole Schritt 4, um den Ausgang des unteren Schalters mit dem unteren"
  //: /line:"Eingang des OR-Gatters zu verbinden."
  //: /line:""
  //: /line:"8) Klicke und halte den unteren Eingang des AND-Gatters, um den Lötkolben"
  //: /line:"anzuwärmen. Ziehe ihn hinunter auf die Mitte des Drahtes, der den unteren "
  //: /line:"Schalter mit dem OR-Gatter verbindet und lasse die Maustaste los. Es wird"
  //: /line:"automatisch ein Klecks Lötzinn erzeugt."
  //: /end
  //: comment g6 /dolink:0 /link:"" @(101,427) /anc:1
  //: /line:"Schalter"
  //: /end
  //: comment g7 /dolink:0 /link:"" @(193,441) /anc:1
  //: /line:"AND-Gatter"
  //: /end
  //: comment g9 /dolink:0 /link:"" @(11,4)
  //: /line:"1) In diesem Beispiel wird die unten gezeigte Schaltung konstruiert."
  //: /line:"Baue die Schaltung im umrahmten Feld rechts unten auf. Beginne mit dem Setzen"
  //: /line:"der Position eines \"AND\"-Gatters durch Klicken mit der rechten Maustaste. Es"
  //: /line:"erscheint ein \"X\" Symbol zur Markierung der Position."
  //: /line:""
  //: /line:"2) Wähle \"AND\" aus dem \"Gatter\"-Submenu nach dem Aufklappen"
  //: /line:"des \"Make\"-Menus mit der rechten Maustaste (oder über die"
  //: /line:"Menuleiste). Damit wird ein AND-Gatter an der ausgewählten Stelle erzeugt."
  //: /line:"Alternativ (und schneller) geht es mit dem Tastenkürzel \"a\"."
  //: /line:""
  //: /line:"3) Wiederhole diese Aktion, diesmal mit Auswahl eines OR-Gatters. Setze es "
  //: /line:"rechts unterhalb der Position des AND-Gatters. Erzeuge das OR-Gatter durch"
  //: /line:"Anwahl von \"OR\" aus dem \"Gatter\"-Submenu des \"Make\"-Menus oder durch Tippen"
  //: /line:"von \"o\"."
  //: /line:""
  //: /line:"4) Verbinde die obere Eingangsleitung des \"OR\"-Gatters mit dem Ausgang"
  //: /line:"des AND-Gatters durch Klicken und Halten des freien Endes des AND-Gatters mit"
  //: /line:"der linken Maustaste. Das ändert den Mauszeiger in einen Lötkolben."
  //: /line:"Nun ziehe den Draht in die Nähe der oberen Eingangsleitung des OR-Gatters und"
  //: /line:"löse die Maustaste. Die Leitungen verbinden sich selbsttätig."
  //: /end
  //: comment g20 /dolink:0 /link:"@T/create_tut.v" @(630,55) /sn:0 /anc:1
  //: /line:"Erzeugen einer Schaltung"
  //: /end
  //: frame g15 @(600,25) /sn:0 /anc:1 /wi:213 /ht:149 /tx:"Tutorien"
  //: comment g17 /dolink:1 /link:"@T/sim_tut.v" @(630,130) /sn:0 /anc:1
  //: /line:"Schaltnetzsimulation"
  //: /end
  //: joint g5 (w6) @(175, 502) /anc:1 /w:[ 1 2 4 -1 ]
  //: comment g14 /dolink:1 /link:"@T/seqsim_tut.v" @(630,145) /sn:0 /anc:1
  //: /line:"Schaltwerkssimulation"
  //: /end
  //: comment g21 /dolink:1 /link:"@T/edit2_tut.v" @(630,85) /sn:0 /anc:1
  //: /line:"Editieren von Gruppen"
  //: /end
  //: comment g0 /dolink:1 /link:"@T/edit1_tut.v" @(630,70) /sn:0 /anc:1
  //: /line:"Grundlegende Editier-Modi"
  //: /end
  //: comment g22 /dolink:0 /link:"" @(605,55) /sn:0 /anc:1
  //: /line:"->"
  //: /end
  //: comment g18 /dolink:1 /link:"@T/welcome_tut.v" @(630,40) /sn:0 /anc:1
  //: /line:"TkGate Einführung"
  //: /end

endmodule
