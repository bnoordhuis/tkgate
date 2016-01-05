//: version "1.6i"
//: property useExtBars = 0
//: property discardChanges = 1

module main;    //: root_module
wire w7;    //: /sn:0 {0}(713,355)(728,355)(728,367){1}
wire s0;    //: {0}(781,302)(811,302){1}
wire w4;    //: /sn:0 {0}(733,337)(733,367){1}
wire c0;    //: {0}(650,264)(735,264)(735,302){1}
//: {2}(737,304)(760,304){3}
//: {4}(735,306)(735,316){5}
wire c1;    //: {0}(731,388)(731,425){1}
//: {2}(733,427)(756,427){3}
//: {4}(731,429)(731,439){5}
wire s1;    //: /dp:1 {0}(777,425)(812,425){1}
wire a1;    //: {0}(645,420)(646,420)(646,419)(669,419){1}
//: {2}(673,419)(681,419){3}
//: {4}(671,421)(671,475)(688,475){5}
wire b1;    //: {0}(647,454)(667,454){1}
//: {2}(665,452)(665,424)(681,424){3}
//: {4}(665,456)(665,480)(688,480){5}
wire w8;    //: /sn:0 {0}(709,478)(724,478)(724,490){1}
wire a0;    //: /dp:1 {0}(650,296)(673,296){1}
//: {2}(677,296)(685,296){3}
//: {4}(675,298)(675,352)(692,352){5}
wire b0;    //: /dp:1 {0}(648,331)(667,331){1}
//: {2}(669,329)(669,301)(685,301){3}
//: {4}(669,333)(669,357)(692,357){5}
wire c2;    //: {0}(814,522)(727,522)(727,511){1}
wire w10;    //: /sn:0 {0}(756,422)(728,422){1}
//: {2}(724,422)(702,422){3}
//: {4}(726,424)(726,439){5}
wire w5;    //: /sn:0 {0}(729,460)(729,490){1}
wire w9;    //: /sn:0 {0}(760,299)(732,299){1}
//: {2}(728,299)(706,299){3}
//: {4}(730,301)(730,316){5}
//: enddecls

  //: joint g8 (c1) @(731, 427) /w:[ 2 1 -1 4 ]
  nand g4 (.I0(w8), .I1(w5), .Z(c2));   //: @(727,501) /sn:0 /R:3 /w:[ 1 1 1 ]
  //: comment g61 /dolink:1 /link:"@T/edwire_tut.v" @(630,100) /sn:0 /anc:1
  //: /line:"Leitungen editieren"
  //: /end
  led g37 (.I(s0));   //: @(818,302) /sn:0 /R:3 /w:[ 1 ] /type:0
  xor g34 (.I0(w10), .I1(c1), .Z(s1));   //: @(767,425) /sn:0 /w:[ 0 3 0 ]
  //: comment g13 /dolink:0 /link:"@T/edit1_tut.v" @(630,70) /sn:0 /anc:1
  //: /line:"Grundlegende Editier-Modi"
  //: /end
  //: comment g3 /dolink:0 /link:"" @(29,277) /anc:1
  //: /line:"5) Öffne das \"Werkzeug\"-Menu und wähle \"Bitbreite\" aus, um das Bandkabel-"
  //: /line:"Werkzeug zu bekommen. Gleichzeitig wird rechts unten neben dem Logo ein Fenster"
  //: /line:"geöffnet, das mit \"Bits:\" gekennzeichnet ist. Trage in das Feld eine Zahl von"
  //: /line:"1 bis 32 ein. Dann klicke auf eine Leitung, um sie auf die angegebene Bitbreite"
  //: /line:"zu bringen."
  //: /line:""
  //: /line:"6) Ein Doppelklick auf eine Leitung öffnet das Eigenschaften-Fenster. Ändere"
  //: /line:"den Signalnamen, mache den Signalnamen sichtbar oder setze die Bitbreite der"
  //: /line:"Leitung. Die Eigenschaftsbox kann auch mit der rechten Maustaste im Klappauf-"
  //: /line:"Menu geöffnet werden."
  //: /line:""
  //: /line:"7) Öffne das \"Werkzeug\"-Menu und wähle \"Bewegen/Verbinden\" aus, um zum Basis-"
  //: /line:"Editierwerkzeug zurückzukehren. Klicke doppelt (oder mit Rechtsklick und "
  //: /line:"\"Eigenschaften\" ...) auf ein XOR-Gatter. Dies öffnet das Eigenschaften-Fenster"
  //: /line:"für das Gatter. Dort kann man den Namen ändern, den Namen sichtbar machen "
  //: /line:"(Rückgängigmachen der \"Verstecken\"-Option) und Ports hinzufügen oder löschen."
  //: /line:""
  //: /line:"8) Schalte mit einem Rechtsklick auf einem Gatter das \"Anker\"-Flag ein. "
  //: /line:"Versuche nun, das Gatter zu bewegen (das sollte nicht gehen). Das kann man"
  //: /line:"benutzen, die Teile der Schaltung festzuzurren, die man möglichst nicht"
  //: /line:"verschoben haben möchte."
  //: /end
  //: comment g2 /dolink:0 /link:"" @(29,38) /anc:1
  //: /line:"1) Mit der linken Maustaste auf das Gatter klicken. Dann ziehe man das Gatter"
  //: /line:"in eine neue Position und beobachte, wie die Leitungen sich mit dem Gatter "
  //: /line:"bewegen."
  //: /line:""
  //: /line:"2) \"Werkzeugmenu\" Öffnen und \"Leitung trennen\" auswählen. Damit wird eine"
  //: /line:"Kneifzange bereitgestellt. Klickt man mit der Kneifzange irgendwo auf einen "
  //: /line:"Draht, so wird er aufgetrennt. Lose Segmente oder Segmente zu einem "
  //: /line:"Verbindungspunkt werden automatisch gelöscht. Den Modus kann man wieder wechseln,"
  //: /line:"indem man mit der rechten Maustaste in einen leeren Bereich klickt und aus dem"
  //: /line:"Klappauf-Menu einen geeigneten Modus auswählt."
  //: /line:""
  //: /line:"3) Öffne das \"Werkzeug\"-Menu und wähle \"Invertieren\" aus, das Invertierer-"
  //: /line:"Werkzeug zeigt sich. Klicke mit der \"Inverter-Blase\" in der Nähe eines Eingangs"
  //: /line:"oder des Ausgangs, so wird eine Invertierung hinzugefügt oder entfernt."
  //: /line:""
  //: /line:"4) Öffne das \"Werkzeug\"-Menu und wähle \"Gatter löschen\" aus; dies stellt ein"
  //: /line:"IC-Entfernungs-Werkzeug bereit. Klicke probeweise damit auf ein Gatter: es wird"
  //: /line:"entfernt. Alternative: Wähle das Gatter aus und drücke die \"Entf\"-Taste oder"
  //: /line:"wähle mit der rechten Maustaste aus dem Klappaufmenu \"Löschen\"."
  //: /end
  nand g1 (.I0(w7), .I1(w4), .Z(c1));   //: @(731,378) /sn:0 /R:3 /w:[ 1 1 0 ]
  //: switch g11 (a1) @(628,420) /sn:0 /w:[ 0 ] /st:0
  //: comment g16 /dolink:1 /link:"@T/module_tut.v" @(630,115) /sn:0 /anc:1
  //: /line:"Module benutzen"
  //: /end
  //: joint g10 (a1) @(671, 419) /w:[ 2 -1 1 4 ]
  nand g28 (.I0(a0), .I1(b0), .Z(w7));   //: @(703,355) /sn:0 /w:[ 5 5 0 ]
  led g19 (.I(s1));   //: @(819,425) /sn:0 /R:3 /w:[ 1 ] /type:0
  //: switch g27 (c0) @(633,264) /sn:0 /w:[ 0 ] /st:0
  //: joint g32 (w9) @(730, 299) /w:[ 1 -1 2 4 ]
  //: joint g38 (b1) @(665, 454) /w:[ 1 2 -1 4 ]
  nand g6 (.I0(a1), .I1(b1), .Z(w8));   //: @(699,478) /sn:0 /w:[ 5 5 0 ]
  nand g9 (.I0(w10), .I1(c1), .Z(w5));   //: @(729,450) /sn:0 /R:3 /w:[ 5 5 0 ]
  //: joint g7 (w10) @(726, 422) /w:[ 1 -1 2 4 ]
  //: frame g15 @(600,25) /sn:0 /anc:1 /wi:213 /ht:149 /tx:"Tutorien"
  //: comment g20 /dolink:1 /link:"@T/create_tut.v" @(630,55) /sn:0 /anc:1
  //: /line:"Erzeugen einer Schaltung"
  //: /end
  nand g31 (.I0(w9), .I1(c0), .Z(w4));   //: @(733,327) /sn:0 /R:3 /w:[ 5 5 0 ]
  led g39 (.I(c2));   //: @(821,522) /sn:0 /R:3 /w:[ 0 ] /type:0
  //: comment g17 /dolink:1 /link:"@T/sim_tut.v" @(630,130) /sn:0 /anc:1
  //: /line:"Schaltnetzsimulation"
  //: /end
  //: switch g25 (a0) @(633,296) /sn:0 /w:[ 0 ] /st:0
  //: joint g29 (a0) @(675, 296) /w:[ 2 -1 1 4 ]
  //: comment g5 /dolink:0 /link:"" @(268,523) /sn:0
  //: /line:""
  //: /end
  //: comment g14 /dolink:1 /link:"@T/seqsim_tut.v" @(630,145) /sn:0 /anc:1
  //: /line:"Schaltwerkssimulation"
  //: /end
  //: switch g36 (b1) @(630,454) /sn:0 /w:[ 0 ] /st:0
  //: comment g21 /dolink:1 /link:"@T/edit2_tut.v" @(630,85) /sn:0 /anc:1
  //: /line:"Editieren von Gruppen"
  //: /end
  xor g24 (.I0(w9), .I1(c0), .Z(s0));   //: @(771,302) /sn:0 /w:[ 0 3 0 ]
  xor g23 (.I0(a0), .I1(b0), .Z(w9));   //: @(696,299) /sn:0 /w:[ 3 3 3 ]
  xor g35 (.I0(a1), .I1(b1), .Z(w10));   //: @(692,422) /sn:0 /w:[ 3 3 3 ]
  //: comment g22 /dolink:0 /link:"" @(605,70) /sn:0 /anc:1
  //: /line:"->"
  //: /end
  //: comment g0 /dolink:0 /link:"" @(8,10) /anc:1
  //: /line:"Dieser Abschnitt führt in die grundlegenden Editiervorgänge von TkGate ein."
  //: /end
  //: switch g26 (b0) @(631,331) /sn:0 /w:[ 0 ] /st:0
  //: comment g18 /dolink:1 /link:"@T/welcome_tut.v" @(630,40) /sn:0 /anc:1
  //: /line:"TkGate Einführung"
  //: /end
  //: joint g30 (b0) @(669, 331) /w:[ -1 2 1 4 ]
  //: joint g33 (c0) @(735, 304) /w:[ 2 1 -1 4 ]

endmodule
