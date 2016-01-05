//: version "2.0-b6"
//: property prefix = "_GG"
//: property title = "edit1_tut.v"
//: property useExtBars = 0
//: property discardChanges = 1
//: property encoding = "utf-8"
//: property locale = "de"

`timescale 1ns/1ns

//: /netlistBegin PAGE1
module PAGE1;    //: root_module
//: enddecls

  //: comment g2 @(10,10) /anc:1
  //: /line:"<h3>Editieren von Gattern</h3>"
  //: /line:""
  //: /line:"Dieser Abschnitt führt in einige grundlegende Editierfunktionen von TkGate für Gatter ein."
  //: /end
  //: comment g10 @(10,310) /sn:0 /R:14 /anc:1
  //: /line:"<tutorial-navigation>"
  //: /end
  //: comment g0 @(144,69) /sn:0 /anc:1
  //: /line:"<img src=bigdetails.gif>"
  //: /end

endmodule
//: /netlistEnd

//: /netlistBegin PAGE6
module PAGE6();
//: interface  /sz:(40, 40) /bd:[ ]
wire w4;    //: /sn:0 {0}(256,171)(271,171){1}
wire w0;    //: /sn:0 {0}(102,172)(117,172){1}
wire w3;    //: /sn:0 {0}(256,166)(271,166){1}
wire w1;    //: /sn:0 {0}(102,177)(117,177){1}
wire w2;    //: /sn:0 {0}(138,175)(153,175){1}
wire w5;    //: /sn:0 {0}(292,169)(307,169){1}
//: enddecls

  //: comment g4 @(247,195) /sn:0 /anc:1
  //: /line:"Verankertes Gatter"
  //: /end
  _GGOR2 #(6) g3 (.I0(w3), .I1(w4), .Z(w5));   //: @(282,169) /sn:0 /anc:1 /w:[ 1 1 0 ]
  //: comment g2 @(10,10) /anc:1
  //: /line:"<h3>Editieren von Gattern</h3> <b>(Verankerung)</b>"
  //: /line:""
  //: /line:"Um zu verhindern, daß ein Gatter aus Versehen bewegt wird, kann man es auswählen und den"
  //: /line:"<img src=anchor.gif bgcolor=gray> Knopf drücken, um einen Anker zu setzen.  Er kann mit dem <img src=unanchor.gif bgcolor=gray> Knopf wieder entfernt werden."
  //: /line:"Festgezurrte Gatter, wie das OR Gatter unten, können nicht bewegt werden, solange"
  //: /line:"der Anker nicht entfernt ist. Versuche die Gatter unten zu bewegen."
  //: /line:""
  //: /end
  _GGAND2 #(6) g1 (.I0(w0), .I1(w1), .Z(w2));   //: @(128,175) /sn:0 /w:[ 1 1 0 ]
  //: comment g6 @(587,112) /sn:0 /anc:1
  //: /line:"<font color=green4>Hinweis: ein alternativer Weg, einen"
  //: /line:"Anker eines Gatters zu setzen oder"
  //: /line:"zu löschen ist ein Rechtsklick"
  //: /line:"und ein Wechsel des Anker-"
  //: /line:"Flags im Popup Menu.</font>"
  //: /end
  //: comment g5 @(81,195) /sn:0 /anc:1
  //: /line:"Nicht verankertes Gatter"
  //: /end
  //: comment g0 @(10,310) /sn:0 /anc:1
  //: /line:"<tutorial-navigation>"
  //: /end

endmodule
//: /netlistEnd

//: /netlistBegin PAGE3
module PAGE3();
//: interface  /sz:(40, 40) /bd:[ ]
reg c0;    //: {0}(18:118,113)(224,113)(224,155){1}
//: {2}(226,157)(249,157){3}
//: {4}(224,159)(224,169){5}
reg a0;    //: {0}(40:118,149)(162,149){1}
//: {2}(166,149)(174,149){3}
//: {4}(164,151)(164,205)(181,205){5}
reg b0;    //: {0}(50:118,184)(156,184){1}
//: {2}(158,182)(158,154)(174,154){3}
//: {4}(158,186)(158,210)(181,210){5}
wire w7;    //: /sn:0 {0}(202,208)(217,208)(217,220){1}
wire s0;    //: {0}(36:270,155)(300,155){1}
wire w4;    //: /sn:0 {0}(222,190)(222,220){1}
wire c1;    //: {0}(99:220,241)(220,260){1}
wire w9;    //: /sn:0 {0}(249,152)(221,152){1}
//: {2}(217,152)(195,152){3}
//: {4}(219,154)(219,169){5}
//: enddecls

  //: LED g37 (s0) @(307,155) /sn:0 /R:3 /w:[ 1 ] /type:0
  //: comment g2 @(11,10) /anc:1
  //: /line:"<h3>Editieren von Gattern</h3> <b>(Verschieben)</b>"
  //: /line:""
  //: /line:"um ein Gatter zu bewegen, wähle es mit <img src=\"mov_curs.gif\" bgcolor=gray> aus und ziehe es an den neuen Ort.  "
  //: /line:"Versuche es mit der Beispielschaltung unten."
  //: /line:""
  //: /end
  _GGNAND2 #(4) g1 (.I0(w7), .I1(w4), .Z(c1));   //: @(220,231) /sn:0 /R:3 /w:[ 1 1 0 ]
  _GGNAND2 #(4) g28 (.I0(a0), .I1(b0), .Z(w7));   //: @(192,208) /sn:0 /w:[ 5 5 0 ]
  //: joint g32 (w9) @(219, 152) /w:[ 1 -1 2 4 ]
  //: SWITCH g27 (c0) @(101,113) /sn:0 /w:[ 0 ] /st:0
  _GGNAND2 #(4) g31 (.I0(w9), .I1(c0), .Z(w4));   //: @(222,180) /sn:0 /R:3 /w:[ 5 5 0 ]
  //: joint g29 (a0) @(164, 149) /w:[ 2 -1 1 4 ]
  //: SWITCH g25 (a0) @(101,149) /sn:0 /w:[ 0 ] /st:0
  _GGXOR2 #(8) g24 (.I0(w9), .I1(c0), .Z(s0));   //: @(260,155) /sn:0 /w:[ 0 3 0 ]
  _GGXOR2 #(8) g23 (.I0(a0), .I1(b0), .Z(w9));   //: @(185,152) /sn:0 /w:[ 3 3 3 ]
  //: SWITCH g26 (b0) @(101,184) /sn:0 /w:[ 0 ] /st:0
  //: comment g0 @(10,310) /sn:0 /anc:1
  //: /line:"<tutorial-navigation>"
  //: /end
  //: joint g33 (c0) @(224, 157) /w:[ 2 1 -1 4 ]
  //: joint g30 (b0) @(158, 184) /w:[ -1 2 1 4 ]

endmodule
//: /netlistEnd

//: /netlistBegin PAGE8
module PAGE8();
//: interface  /sz:(40, 40) /bd:[ ]
wire w6;    //: /sn:0 {0}(39,221)(54,221){1}
wire w7;    //: /sn:0 {0}(41,265)(56,265){1}
wire w16;    //: /sn:0 {0}(154,267)(169,267){1}
wire w14;    //: /sn:0 {0}(169,222)(154,222){1}
wire w19;    //: /sn:0 {0}(169,277)(154,277){1}
wire w15;    //: /sn:0 {0}(154,262)(169,262){1}
wire w4;    //: /sn:0 {0}(39,218)(54,218){1}
wire w0;    //: /sn:0 {0}(37,166)(52,166){1}
wire w3;    //: /sn:0 {0}(39,214)(54,214){1}
wire w21;    //: /sn:0 {0}(152,167)(167,167){1}
wire w20;    //: /sn:0 {0}(41,274)(56,274){1}
wire w23;    //: /sn:0 {0}(188,169)(203,169){1}
wire w1;    //: /sn:0 {0}(37,171)(52,171){1}
wire w18;    //: /sn:0 {0}(190,269)(205,269){1}
wire w8;    //: /sn:0 {0}(41,268)(56,268){1}
wire w17;    //: /sn:0 {0}(169,272)(154,272){1}
wire w22;    //: /sn:0 {0}(152,172)(167,172){1}
wire w12;    //: /sn:0 {0}(154,217)(169,217){1}
wire w11;    //: /sn:0 {0}(154,212)(169,212){1}
wire w2;    //: /sn:0 {0}(73,169)(88,169){1}
wire w10;    //: /sn:0 {0}(41,271)(56,271){1}
wire w13;    //: /sn:0 {0}(190,217)(205,217){1}
wire w5;    //: /sn:0 {0}(75,218)(90,218){1}
wire w9;    //: /sn:0 {0}(77,270)(92,270){1}
//: enddecls

  _GGAND4 #(10) g4 (.I0(w7), .I1(w8), .I2(w10), .I3(w20), .Z(w9));   //: @(67,270) /sn:0 /w:[ 1 1 1 1 0 ]
  //: comment g8 @(149,126) /sn:0 /anc:1
  //: /line:"Gatter mit"
  //: /line:"Erweiterungsbalken"
  //: /end
  _GGAND3 #(8) g3 (.I0(w3), .I1(w4), .I2(w6), .Z(w5));   //: @(65,218) /sn:0 /w:[ 1 1 1 0 ]
  //: comment g2 @(11,13) /anc:1
  //: /line:"<h3>Editieren von Gattern</h3> <b>(Eingänge hinzufügen)</b>"
  //: /line:""
  //: /line:"Einige Gatter (AND Gatter z.B.) können eine variable Anzahl von Eingängen oder Ausgängen haben. Um diesen Gattern"
  //: /line:"Eingänge oder AusgÄnge hinzuzufügen, wählt man das Gatter aus und drückt den <img src=addport.gif bgcolor=gray> Knopf. Einige Gatter können auch"
  //: /line:"verschiedene Methoden zum Hinzufügen von Ports bieten, die dann in der Eigenschaftsbox"
  //: /line:"des Gatters angeboten werden. Versuche, zu den Gattern unten Eingänge hinzuzufügen"
  //: /line:"oder erzeuge ein neues Gatter und versieh es mit zusätzlichen Eingängen."
  //: /end
  _GGAND2 #(6) g1 (.I0(w0), .I1(w1), .Z(w2));   //: @(63,169) /sn:0 /w:[ 1 1 0 ]
  _GGAND4 #(10) g6 (.I0(w15), .I1(w16), .I2(w17), .I3(w19), .Z(w18));   //: @(180,269) /sn:0 /w:[ 1 1 0 0 0 ] /eb:1
  //: comment g7 @(26,127) /sn:0 /anc:1
  //: /line:"Gatter ohne"
  //: /line:"Erweiterungsbalken"
  //: /end
  _GGAND2 #(6) g9 (.I0(w21), .I1(w22), .Z(w23));   //: @(178,169) /sn:0 /w:[ 1 1 0 ] /eb:1
  _GGAND3 #(8) g5 (.I0(w11), .I1(w12), .I2(w14), .Z(w13));   //: @(180,217) /sn:0 /w:[ 1 1 0 0 ] /eb:1
  //: comment g0 @(10,310) /sn:0 /anc:1
  //: /line:"<tutorial-navigation>"
  //: /end

endmodule
//: /netlistEnd

//: /netlistBegin PAGE7
module PAGE7();
//: interface  /sz:(40, 40) /bd:[ ]
//: enddecls

  //: frame g3 @(298,213) /sn:0 /wi:167 /ht:69 /tx:"This is a frame"
  //: comment g2 @(10,10) /anc:1
  //: /line:"<h3>Editieren von Gattern</h3> <b>(Kommentare und Rahmen)</b>"
  //: /line:""
  //: /line:"Es gibt zwei spezielle \"Gatter\", die man zur Beschriftung der Schaltung nutzen kann: Kommentare und Rahmen."
  //: /line:"Keines dieser \"Gatter\" hat irgendeinen Effekt auf die Simulationsergebnisse."
  //: /line:""
  //: /line:"Kommentare sind Textblöcke wie die, mit denen dieser Lehrgang erzeugt ist.  Kommentare werden durch Rechtsklick"
  //: /line:"und Auswahl von <font color=red2>Bausteine &rarr;   Kommentar</font> aus dem Popup Menu erzeugt. Wird ein Kommentar erzeugt, so erscheint"
  //: /line:"die Eigenschaftsbox sofort und erlaubt, den Text des Kommentars einzugeben.  Kommentare können eine begrenzte"
  //: /line:"Zahl von HTML-Tags einschließlich Links zu anderen Schaltungsdateien enthalten, oder Kurzschlußzugänge zu"
  //: /line:"bestimmten Gattern der aktuellen Schaltung."
  //: /line:""
  //: /line:"Rahmen sind benannte Boxen, mit denen Gruppen zusammengehöriger Schaltungselemente zusammengefaßt werden"
  //: /line:"können, wenn sich die Erzeugung eines eigenen Moduls dafür nicht lohnt. Öffne die Eigenschaftsbox durch Doppelklick"
  //: /line:"auf eine Kante und gib die Zeichenkette zur Beschreibung ein.  Verändere die Größe durch Klicken und Ziehen an einer Ecke."
  //: /line:""
  //: /end
  //: comment g1 @(72,233) /sn:0
  //: /line:"Dies ist ein Kommentar"
  //: /line:"mit <b>fettem</b> Text."
  //: /end
  //: comment g0 @(10,310) /sn:0 /anc:1
  //: /line:"<tutorial-navigation>"
  //: /end

endmodule
//: /netlistEnd

//: /netlistBegin PAGE5
module PAGE5();
//: interface  /sz:(40, 40) /bd:[ ]
wire w0;    //: /sn:0 {0}(118,149)(133,149){1}
wire w1;    //: /sn:0 {0}(118,154)(133,154){1}
wire w2;    //: /sn:0 {0}(154,152)(169,152){1}
//: enddecls

  //: comment g2 @(10,10) /anc:1
  //: /line:"<h3>Editieren von Gattern</h3> <b>(Drehungen)</b>"
  //: /line:""
  //: /line:"Die Ausrichtung eines Gatters kann geändert werden, indem man das Gatter auswählt und den <img src=\"edit_rotate.gif\" bgcolor=gray> Knopf"
  //: /line:"(oder die <font color=red2>Tab</font> Taste) drückt, um es gegen den Uhrzeigersinn zu drehen. Den  <img src=\"edit_brotate.gif\" bgcolor=gray> Knopf (oder die <font color=red2>Shift-Tab</font>"
  //: /line:"Tastenkombination) nimmt man, um im Uhrzeigersinn zu drehen.  Versuche das mit dem AND Gatter unten."
  //: /end
  _GGAND2 #(6) g1 (.I0(w0), .I1(w1), .Z(w2));   //: @(144,152) /sn:0 /w:[ 1 1 0 ]
  //: comment g6 @(567,92) /sn:0 /anc:1
  //: /line:"<font color=green4>Hinweis: Genauso gut kann man den <img src=\"rotation0.gif\" bgcolor=gray>"
  //: /line:"Knopf drücken, um die Standard-"
  //: /line:"orientierung für neue erzeugte Gatter"
  //: /line:"festzulegen.</font>"
  //: /end
  //: comment g0 @(10,310) /sn:0 /anc:1
  //: /line:"<tutorial-navigation>"
  //: /end

endmodule
//: /netlistEnd

//: /netlistBegin PAGE2
module PAGE2();
//: interface  /sz:(40, 40) /bd:[ ]
reg c0;    //: {0}(18:125,126)(231,126)(231,168){1}
//: {2}(233,170)(256,170){3}
//: {4}(231,172)(231,183){5}
reg a0;    //: {0}(40:125,162)(169,162){1}
//: {2}(173,162)(181,162){3}
//: {4}(171,164)(171,218)(188,218){5}
reg b0;    //: {0}(50:125,197)(163,197){1}
//: {2}(165,195)(165,167)(181,167){3}
//: {4}(165,199)(165,223)(188,223){5}
wire w7;    //: /sn:0 {0}(209,221)(224,221)(224,233){1}
wire s0;    //: {0}(36:277,168)(307,168){1}
wire w4;    //: /sn:0 {0}(229,204)(229,233){1}
wire c1;    //: {0}(99:227,254)(227,273){1}
wire w9;    //: /sn:0 {0}(256,165)(228,165){1}
//: {2}(224,165)(202,165){3}
//: {4}(226,167)(226,183){5}
//: enddecls

  //: LED g37 (s0) @(314,168) /sn:0 /R:3 /w:[ 1 ] /type:0
  //: comment g2 @(10,10) /anc:1
  //: /line:"<h3>Editieren von Gattern</h3> <b>(Eigenschaften)</b>"
  //: /line:""
  //: /line:"Die Eigenschaftsbox eines Gatters öffnet man durch Doppelklick darauf.  Versuche das"
  //: /line:"bei einigen Gattern der untenstehenden Schaltung. In der Eigensschaftsbox kann"
  //: /line:"die Bezeichnng eines Gatters geändert werden. Sie kann auch durch die"
  //: /line:"\"Namen verbergen\"-Eigenschaft sichtbar oder unsichtbar gemacht werden."
  //: /end
  _GGNAND2 #(4) g1 (.I0(w7), .I1(w4), .Z(c1));   //: @(227,244) /sn:0 /R:3 /w:[ 1 1 0 ]
  _GGNAND2 #(4) g28 (.I0(a0), .I1(b0), .Z(w7));   //: @(199,221) /sn:0 /w:[ 5 5 0 ]
  //: joint g32 (w9) @(226, 165) /w:[ 1 -1 2 4 ]
  //: SWITCH g27 (c0) @(108,126) /sn:0 /w:[ 0 ] /st:0
  //: comment g6 @(548,104) /sn:0 /anc:1
  //: /line:"<font color=green4>Hinweis: Viele Eigenschaften sind gatter-"
  //: /line:"spezifisch. Öffne die Eigenschaftsboxen"
  //: /line:"unterschiedlicher Gattertypen, um zu sehen,"
  //: /line:"welche Eigenschaften geändert werden"
  //: /line:"können.</font>"
  //: /end
  _GGNAND2 #(4) g31 (.I0(w9), .I1(c0), .Z(w4));   //: @(229,194) /sn:0 /R:3 /w:[ 5 5 0 ]
  //: joint g29 (a0) @(171, 162) /w:[ 2 -1 1 4 ]
  //: SWITCH g25 (a0) @(108,162) /sn:0 /w:[ 0 ] /st:0
  _GGXOR2 #(8) g24 (.I0(w9), .I1(c0), .Z(s0));   //: @(267,168) /sn:0 /w:[ 0 3 0 ]
  _GGXOR2 #(8) g23 (.I0(a0), .I1(b0), .Z(w9));   //: @(192,165) /sn:0 /w:[ 3 3 3 ]
  //: SWITCH g26 (b0) @(108,197) /sn:0 /w:[ 0 ] /st:0
  //: comment g0 @(10,310) /sn:0 /anc:1
  //: /line:"<tutorial-navigation>"
  //: /end
  //: joint g33 (c0) @(231, 170) /w:[ 2 1 -1 4 ]
  //: joint g30 (b0) @(165, 197) /w:[ -1 2 1 4 ]

endmodule
//: /netlistEnd

//: /netlistBegin PAGE4
module PAGE4();
//: interface  /sz:(40, 40) /bd:[ ]
reg c0;    //: {0}(18:125,126)(231,126)(231,168){1}
//: {2}(233,170)(256,170){3}
//: {4}(231,172)(231,183){5}
reg a0;    //: {0}(40:125,162)(169,162){1}
//: {2}(173,162)(181,162){3}
//: {4}(171,164)(171,218)(188,218){5}
reg b0;    //: {0}(50:125,197)(163,197){1}
//: {2}(165,195)(165,167)(181,167){3}
//: {4}(165,199)(165,223)(188,223){5}
wire w7;    //: /sn:0 {0}(209,221)(224,221)(224,233){1}
wire s0;    //: {0}(36:277,168)(307,168){1}
wire w4;    //: /sn:0 {0}(229,204)(229,233){1}
wire c1;    //: {0}(99:227,254)(227,273){1}
wire w9;    //: /sn:0 {0}(256,165)(228,165){1}
//: {2}(224,165)(202,165){3}
//: {4}(226,167)(226,183){5}
//: enddecls

  //: LED g37 (s0) @(314,168) /sn:0 /R:3 /w:[ 1 ] /type:0
  //: comment g2 @(10,10) /anc:1
  //: /line:"<h3>Editieren von Gattern</h3> <b>(Löschen)</b>"
  //: /line:""
  //: /line:"Um ein Gatter zu löschen, wird es mit dem <img src=\"mov_curs.gif\" bgcolor=gray> Werkzeug ausgewählt und die <font color=red2>Entf</font> Taste gedrückt."
  //: /line:"Versuche das bei der Beispielschaltung unten."
  //: /end
  _GGNAND2 #(4) g1 (.I0(w7), .I1(w4), .Z(c1));   //: @(227,244) /sn:0 /R:3 /w:[ 1 1 0 ]
  _GGNAND2 #(4) g28 (.I0(a0), .I1(b0), .Z(w7));   //: @(199,221) /sn:0 /w:[ 5 5 0 ]
  //: comment g10 @(10,310) /sn:0 /R:14 /anc:1
  //: /line:"<tutorial-navigation>"
  //: /end
  //: joint g32 (w9) @(226, 165) /w:[ 1 -1 2 4 ]
  //: SWITCH g27 (c0) @(108,126) /sn:0 /w:[ 0 ] /st:0
  _GGNAND2 #(4) g31 (.I0(w9), .I1(c0), .Z(w4));   //: @(229,194) /sn:0 /R:3 /w:[ 5 5 0 ]
  //: joint g29 (a0) @(171, 162) /w:[ 2 -1 1 4 ]
  //: SWITCH g25 (a0) @(108,162) /sn:0 /w:[ 0 ] /st:0
  _GGXOR2 #(8) g24 (.I0(w9), .I1(c0), .Z(s0));   //: @(267,168) /sn:0 /w:[ 0 3 0 ]
  _GGXOR2 #(8) g23 (.I0(a0), .I1(b0), .Z(w9));   //: @(192,165) /sn:0 /w:[ 3 3 3 ]
  //: SWITCH g26 (b0) @(108,197) /sn:0 /w:[ 0 ] /st:0
  //: joint g33 (c0) @(231, 170) /w:[ 2 1 -1 4 ]
  //: joint g30 (b0) @(165, 197) /w:[ -1 2 1 4 ]

endmodule
//: /netlistEnd


`timescale 1ns/1ns


//: /builtinBegin
module _GGXOR2 #(.Diz(1)) (I0, I1, Z);
output Z;
input I0;
input I1;

  assign #Diz Z = I0 ^ I1;

endmodule
//: /builtinEnd


//: /builtinBegin
module _GGNAND2 #(.Diz(1)) (I0, I1, Z);
output Z;
input I0;
input I1;

  assign #Diz Z = ~(I0 & I1);

endmodule
//: /builtinEnd


//: /builtinBegin
module _GGAND4 #(.Diz(1)) (I0, I1, I2, I3, Z);
output Z;
input I0;
input I1;
input I2;
input I3;

  assign #Diz Z = I0 & I1 & I2 & I3;

endmodule
//: /builtinEnd


//: /builtinBegin
module _GGAND2 #(.Diz(1)) (I0, I1, Z);
output Z;
input I0;
input I1;

  assign #Diz Z = I0 & I1;

endmodule
//: /builtinEnd


//: /builtinBegin
module _GGAND3 #(.Diz(1)) (I0, I1, I2, Z);
output Z;
input I0;
input I1;
input I2;

  assign #Diz Z = I0 & I1 & I2;

endmodule
//: /builtinEnd


//: /builtinBegin
module _GGOR2 #(.Diz(1)) (I0, I1, Z);
output Z;
input I0;
input I1;

  assign #Diz Z = I0 | I1;

endmodule
//: /builtinEnd

