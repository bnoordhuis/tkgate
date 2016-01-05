//: version "1.6i"
//: property useExtBars = 0
//: property discardChanges = 1

module foo();
//: interface  /sz:(40, 40) /bd:[ ]
//: enddecls

  //: comment g61 /dolink:1 /link:"@T/edwire_tut.v" @(630,100) /sn:0
  //: /line:"Leitungen editieren"
  //: /end
  //: comment g3 /dolink:1 /link:"@T/edit1_tut.v" @(630,70) /sn:0
  //: /line:"Grundlegende Editier-Modi"
  //: /end
  //: frame g1 @(409,199) /sn:0 /wi:404 /ht:316 /tx:"Hier Ports des Moduls erzeugen"
  //: comment g16 /dolink:0 /link:"@T/module_tut.v" @(630,115) /sn:0
  //: /line:"Module benutzen"
  //: /end
  //: comment g20 /dolink:1 /link:"@T/create_tut.v" @(630,55) /sn:0
  //: /line:"Erzeugen einer Schaltung"
  //: /end
  //: frame g15 @(600,25) /sn:0 /wi:213 /ht:149 /tx:"Tutorien"
  //: comment g17 /dolink:1 /link:"@T/sim_tut.v" @(630,130) /sn:0
  //: /line:"Schaltnetzsimulation"
  //: /end
  //: comment g14 /dolink:1 /link:"@T/seqsim_tut.v" @(630,145) /sn:0
  //: /line:"Schaltwerkssimulation"
  //: /end
  //: comment g21 /dolink:1 /link:"@T/edit2_tut.v" @(630,85) /sn:0
  //: /line:"Editieren von Gruppen"
  //: /end
  //: comment g0 /dolink:0 /link:"" @(26,29) /anc:1
  //: /line:"10) Dies ist die Definition des \"foo\""
  //: /line:"Moduls.  Die \"Ports\" Liste in der linken"
  //: /line:"unteren Ecke des Hauptfensters zeigt die"
  //: /line:"Ports an, die wir in der Standard Modulinstanz"
  //: /line:"definiert haben. Um eine interne Referenz auf"
  //: /line:"einen Port zu erzeugen, klicke mit der Maus"
  //: /line:"(Setzen einer Markierung) und wähle \"Modul Eingang\","
  //: /line:"\"Modul Ausgang\" oder \"Modul InOut\" aus dem"
  //: /line:"\"Modul\" Untermenu des \"Make\"-Menus. Es"
  //: /line:"erscheint ein Dialogfenster, in dem man den"
  //: /line:"Namen des Ports setzen kann. Gib einen Namen ein"
  //: /line:"und drücke \"RETURN\"."
  //: /line:""
  //: /line:"11) Man kann auch eine neue Moduldefinition"
  //: /line:"erzeugen, ohne eine Instanz zu erzeugen, indem"
  //: /line:"man \"Neu...\" aus dem \"Module\"-Menu auswählt."
  //: /line:"Dadurch erhält man ein Dialogfenster, in dem"
  //: /line:"nach einem Modulnamen gefragt wird. Gib"
  //: /line:"\"bar\" ein und drücke RETURN."
  //: /line:""
  //: /line:"12) Man kann Module umbenennen, kopieren und"
  //: /line:"löschen, indem man die anderen Kommandos im \"Modul\"-"
  //: /line:"Menu benutzt."
  //: /line:""
  //: /line:"13) Falls dieser Block durch Doppelklick"
  //: /line:"auf  \"foo\" in der Modulliste geöffnet wurde,"
  //: /line:"kann man zum Hauptmodul zurückkehren, indem man auf"
  //: /line:"\"main\" doppelklickt. Falls dieser Block mit dem"
  //: /line:"\"Öffnen\" Kommando aus dem \"Modul\"-Menu geöffnet"
  //: /line:"wurde, kann man zur nächsthöheren Ebene gelangen,"
  //: /line:"indem \"Schließen\" im \"Modul\"-Menu ausgewählt wird"
  //: /line:"oder ein Doppelklick auf \"main\" erfolgt."
  //: /end
  //: comment g22 /dolink:0 /link:"" @(605,115) /sn:0
  //: /line:"->"
  //: /end
  //: comment g18 /dolink:1 /link:"@T/welcome_tut.v" @(630,40) /sn:0
  //: /line:"TkGate Einführung"
  //: /end

endmodule

module main;    //: root_module
//: enddecls

  //: comment g61 /dolink:1 /link:"@T/edwire_tut.v" @(630,100) /sn:0 /anc:1
  //: /line:"Leitungen editieren"
  //: /end
  //: comment g3 /dolink:1 /link:"@T/edit1_tut.v" @(630,70) /sn:0 /anc:1
  //: /line:"Grundlegende Editier-Modi"
  //: /end
  //: frame g2 @(477,189) /sn:0 /wi:330 /ht:350 /tx:"Freies Feld für Modulinstanzen"
  //: comment g1 /dolink:0 /link:"" @(11,34) /anc:1
  //: /line:"1) Klicke mit der linken Maustaste in der Mitte der leeren Fläche."
  //: /line:"Wähle nun \"Modul-Instanz\" aus dem \"Module\"-Untermenu des \"Make\"-Menus. Damit"
  //: /line:"wird eine Modul-Instanz erzeugt, eine Dialogbox erscheint, in der der"
  //: /line:"\"Funktion\"-Eintrag aktiviert ist. Trage dort \"foo\" in Kleinbuchstaben ein und"
  //: /line:"drücke RETURN. Damit wird der Instanzen-Typ auf \"foo\" gesetzt."
  //: /line:" "
  //: /line:"2) Drücke die linke Maustaste in der Nähe der unteren rechten Ecke der"
  //: /line:"Modulinstanz und ziehe sie nach unten und nach rechts, um den Kasten zu"
  //: /line:"vergrößern."
  //: /end
  //: comment g16 /dolink:0 /link:"@T/module_tut.v" @(630,115) /sn:0 /anc:1
  //: /line:"Module benutzen"
  //: /end
  //: comment g6 /dolink:0 /link:"" @(10,148) /anc:1
  //: /line:"3) Klicke mit der rechten Maustaste auf den Block nahe der linken"
  //: /line:"Kante und wähle \"Eingang hinzufügen\" aus dem hochkommenden Menu. Es "
  //: /line:"öffnet sich eine Dialogbox, in der der Eintrag für \"Port Name\" "
  //: /line:"aktiviert ist. Tippe ein \"A\" und RETURN. Wiederhole den Vorgang"
  //: /line:"und füge die Eingänge \"B\" und \"C\" auf der gleichen Seite hinzu."
  //: /line:"Wiederhole den gleichen Vorgang auf der rechten Seite und füge"
  //: /line:"diesmal Ausgänge hinzu: erzeuge \"X\", \"Y\" und \"Z\" auf der rechten"
  //: /line:"Seite des Blocks."
  //: /line:""
  //: /line:"4) Klicke auf einen der neu hinzugefügten Ports nahe der Modul-"
  //: /line:"Kante. Er kann dann entlang der Kante, an der er hinzugefügt "
  //: /line:"wurde, verschoben werden."
  //: /line:""
  //: /line:"5) Klicke mit der rechten Maustaste auf den Block in der Nähe"
  //: /line:"eines der Ports und wähle \"Typ ändern\" aus dem hochkommenden Menu."
  //: /line:"Das ändert die Richtung des selektierten Ports. Wiederhole"
  //: /line:"diese Aktion und ändere in einen der drei Port-Typen \"Eingang\","
  //: /line:"\"Ausgang\" oder \"InOut\"."
  //: /line:""
  //: /line:"6) Wähle \"Leitung trennen\" aus dem \"Werkzeug\"-Menu und klicke "
  //: /line:"auf den \"Z\"-Draht nahe an der Kante der Modulinstanz. Damit"
  //: /line:"wird der Port gelöscht. "
  //: /line:""
  //: /line:"7) Wähle \"Bewegen/Verbinden\" aus dem \"Werkzeug\"-Menu, um in den"
  //: /line:"Standard-Editiermodus zu gelangen. Wähle nun eine neue Position "
  //: /line:"mit der Maus und erzeuge eine neue Modulinstanz vom Typ \"foo\"."
  //: /line:"Beachte, daß die Instanzgröße und die Leitungspositionen nicht "
  //: /line:"kopiert werden."
  //: /line:""
  //: /line:"8) Selektiere wieder die Instanz, zu der Eingänge und "
  //: /line:"Ausgänge hinzugefügt wurden, und wähle \"Schnittstelle belegen\""
  //: /line:"aus dem \"Modul\"-Menu. Erzeuge nun wieder eine neue Modul-Instanz"
  //: /line:"des \"foo\"-Moduls. Jetzt wird bei jeder neu erzeugten \"foo\"-Instanz"
  //: /line:"auch die Schnittstelle mit kopiert."
  //: /line:""
  //: /line:"9) Öffne nun das \"foo\"-Modul. Das geht entweder durch Auswahl "
  //: /line:"einer der Modul-Instanzen und nachfolgender Auswahl von \"Öffnen\""
  //: /line:"aus dem \"Modul\"-Menu oder durch Doppelklick auf den Eintrag \"foo\""
  //: /line:"in der Modulliste auf der linken Seite im Hauptfenster."
  //: /end
  //: comment g20 /dolink:1 /link:"@T/create_tut.v" @(630,55) /sn:0 /anc:1
  //: /line:"Erzeugen einer Schaltung"
  //: /end
  //: frame g15 @(600,25) /sn:0 /anc:1 /wi:213 /ht:149 /tx:"Tutorien"
  //: comment g17 /dolink:1 /link:"@T/sim_tut.v" @(630,130) /sn:0 /anc:1
  //: /line:"Schaltnetzsimulation"
  //: /end
  //: comment g14 /dolink:1 /link:"@T/seqsim_tut.v" @(630,145) /sn:0 /anc:1
  //: /line:"Schaltwerkssimulation"
  //: /end
  //: comment g21 /dolink:1 /link:"@T/edit2_tut.v" @(630,85) /sn:0 /anc:1
  //: /line:"Editieren von Gruppen"
  //: /end
  //: comment g0 /dolink:0 /link:"" @(22,9) /anc:1
  //: /line:"Dieses Tutorium zeigt die Verwendung von Modulen in TkGate."
  //: /end
  //: comment g22 /dolink:0 /link:"" @(605,115) /sn:0 /anc:1
  //: /line:"->"
  //: /end
  //: comment g18 /dolink:1 /link:"@T/welcome_tut.v" @(630,40) /sn:0 /anc:1
  //: /line:"TkGate Einführung"
  //: /end

endmodule
