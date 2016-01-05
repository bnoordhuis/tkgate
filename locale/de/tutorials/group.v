//: version "2.0-b6"
//: property prefix = "_GG"
//: property title = "edit2_tut.v"
//: property useExtBars = 0
//: property discardChanges = 1
//: property encoding = "utf-8"
//: property locale = "de"

`timescale 1ns/1ns

//: /netlistBegin PAGE1
module PAGE1;    //: root_module
//: enddecls

  //: comment g1 @(140,67) /sn:0 /anc:1
  //: /line:"<img src=bigcircuit.gif>"
  //: /end
  //: comment g5 @(10,10) /anc:1
  //: /line:"<h3>Editieren von Bausteingruppen</h3>"
  //: /line:""
  //: /line:"In diesem Abschnitt lernen wir, wie Gruppen von Gattern gemeinsam editiert werden können."
  //: /end
  //: comment g0 @(10,310) /sn:0 /R:14 /anc:1
  //: /line:"<tutorial-navigation>"
  //: /end

endmodule
//: /netlistEnd

//: /netlistBegin PAGE3
module PAGE3();
//: interface  /sz:(40, 40) /bd:[ ]
reg c0;    //: {0}(18:183,147)(289,147)(289,189){1}
//: {2}(291,191)(314,191){3}
//: {4}(289,193)(289,210){5}
reg a0;    //: {0}(40:183,183)(227,183){1}
//: {2}(231,183)(239,183){3}
//: {4}(229,185)(229,239)(246,239){5}
reg b0;    //: {0}(50:183,218)(221,218){1}
//: {2}(223,216)(223,188)(239,188){3}
//: {4}(223,220)(223,244)(246,244){5}
wire w7;    //: /sn:0 {0}(267,242)(282,242)(282,254){1}
wire s0;    //: {0}(36:335,189)(365,189){1}
wire w4;    //: /sn:0 {0}(287,231)(287,254){1}
wire c1;    //: {0}(99:285,275)(285,294){1}
wire w9;    //: /sn:0 {0}(314,186)(286,186){1}
//: {2}(282,186)(260,186){3}
//: {4}(284,188)(284,210){5}
//: enddecls

  //: LED g37 (s0) @(372,189) /sn:0 /R:3 /w:[ 1 ] /type:0
  _GGNAND2 #(4) g1 (.I0(w7), .I1(w4), .Z(c1));   //: @(285,265) /sn:0 /R:3 /w:[ 1 1 0 ]
  _GGNAND2 #(4) g28 (.I0(a0), .I1(b0), .Z(w7));   //: @(257,242) /sn:0 /w:[ 5 5 0 ]
  //: joint g32 (w9) @(284, 186) /w:[ 1 -1 2 4 ]
  //: SWITCH g27 (c0) @(166,147) /sn:0 /w:[ 0 ] /st:0
  _GGNAND2 #(4) g31 (.I0(w9), .I1(c0), .Z(w4));   //: @(287,221) /sn:0 /R:3 /w:[ 5 5 0 ]
  //: joint g29 (a0) @(229, 183) /w:[ 2 -1 1 4 ]
  //: SWITCH g25 (a0) @(166,183) /sn:0 /w:[ 0 ] /st:0
  //: comment g5 @(10,10) /anc:1
  //: /line:"<h3>Editieren von Bausteingruppen</h3> <b>(inkrementelle Auswahl)</b>"
  //: /line:""
  //: /line:"Es ist möglich, zur aktuellen Auswahl Gatter hinzuzufügen oder zu entfernen. Dazu hält man die <font color=red2>Strg</font> Taste gedrückt, während"
  //: /line:"auf das Gatter geklickt wird.  Wähle mit der Auswahlbox die Gatter unten aus und klicke dann auf einige, während die <font color=red2>Strg</font> Taste"
  //: /line:"gedrückt bleibt.  Gib die <font color=red2>Strg</font> Taste frei und versuche dann, die Auswahl mit nur einigen ausgewählten Gattern zu bewegen."
  //: /end
  _GGXOR2 #(8) g24 (.I0(w9), .I1(c0), .Z(s0));   //: @(325,189) /sn:0 /w:[ 0 3 0 ]
  _GGXOR2 #(8) g23 (.I0(a0), .I1(b0), .Z(w9));   //: @(250,186) /sn:0 /w:[ 3 3 3 ]
  //: comment g0 @(10,310) /sn:0 /R:14 /anc:1
  //: /line:"<tutorial-navigation>"
  //: /end
  //: SWITCH g26 (b0) @(166,218) /sn:0 /w:[ 0 ] /st:0
  //: joint g33 (c0) @(289, 191) /w:[ 2 1 -1 4 ]
  //: joint g30 (b0) @(223, 218) /w:[ -1 2 1 4 ]

endmodule
//: /netlistEnd

//: /netlistBegin PAGE5
module PAGE5();
//: interface  /sz:(40, 40) /bd:[ ]
reg c0;    //: {0}(18:167,153)(273,153)(273,195){1}
//: {2}(275,197)(298,197){3}
//: {4}(273,199)(273,216){5}
reg a0;    //: {0}(40:167,189)(189,189)(189,189)(211,189){1}
//: {2}(215,189)(223,189){3}
//: {4}(213,191)(213,245)(230,245){5}
reg b0;    //: {0}(50:167,224)(186,224)(186,224)(205,224){1}
//: {2}(207,222)(207,194)(223,194){3}
//: {4}(207,226)(207,250)(230,250){5}
wire w7;    //: /sn:0 {0}(251,248)(266,248)(266,260){1}
wire s0;    //: {0}(36:319,195)(349,195){1}
wire w4;    //: /sn:0 {0}(271,237)(271,260){1}
wire c1;    //: {0}(99:269,281)(269,300){1}
wire w9;    //: /sn:0 {0}(298,192)(270,192){1}
//: {2}(266,192)(244,192){3}
//: {4}(268,194)(268,216){5}
//: enddecls

  //: LED g37 (s0) @(356,195) /sn:0 /R:3 /w:[ 1 ] /type:0
  //: comment g2 @(570,113) /sn:0 /anc:1
  //: /line:"<font color=green4>Hinweis: Enthält eine Auswahl"
  //: /line:"irgendwelche verankerten Gatter, so" 
  //: /line:"verhält sich die gesamte Auswahl,"
  //: /line:"als wäre sie verankert.</font>"
  //: /end
  _GGNAND2 #(4) g1 (.I0(w7), .I1(w4), .Z(c1));   //: @(269,271) /sn:0 /R:3 /w:[ 1 1 0 ]
  _GGNAND2 #(4) g28 (.I0(a0), .I1(b0), .Z(w7));   //: @(241,248) /sn:0 /w:[ 5 5 0 ]
  //: joint g32 (w9) @(268, 192) /w:[ 1 -1 2 4 ]
  //: SWITCH g27 (c0) @(150,153) /sn:0 /w:[ 0 ] /st:0
  _GGNAND2 #(4) g31 (.I0(w9), .I1(c0), .Z(w4));   //: @(271,227) /sn:0 /R:3 /w:[ 5 5 0 ]
  //: joint g29 (a0) @(213, 189) /w:[ 2 -1 1 4 ]
  //: SWITCH g25 (a0) @(150,189) /sn:0 /w:[ 0 ] /st:0
  //: comment g5 @(10,10) /anc:1
  //: /line:"<h3>Editieren von Bausteingruppen</h3> <b>(Drehen und Verankern)</b>"
  //: /line:""
  //: /line:"Eine Bausteingruppe kann (mit <img src=edit_rotate.gif bgcolor=gray> und <img src=edit_brotate.gif bgcolor=gray>) gedreht und (mit <img src=anchor.gif bgcolor=gray> und <img src=unanchor.gif bgcolor=gray>) verankert werden, und das genauso, wie es im Abschnitt"
  //: /line:"<a href=gates.v>Gatter editieren</a> für einzelne Gatter geschah.  Versuche es mit der Schaltung unten."
  //: /end
  _GGXOR2 #(8) g24 (.I0(w9), .I1(c0), .Z(s0));   //: @(309,195) /sn:0 /w:[ 0 3 0 ]
  _GGXOR2 #(8) g23 (.I0(a0), .I1(b0), .Z(w9));   //: @(234,192) /sn:0 /w:[ 3 3 3 ]
  //: SWITCH g26 (b0) @(150,224) /sn:0 /w:[ 0 ] /st:0
  //: comment g0 @(10,310) /sn:0 /R:14 /anc:1
  //: /line:"<tutorial-navigation>"
  //: /end
  //: joint g33 (c0) @(273, 197) /w:[ 2 1 -1 4 ]
  //: joint g30 (b0) @(207, 224) /w:[ -1 2 1 4 ]

endmodule
//: /netlistEnd

//: /netlistBegin PAGE2
module PAGE2();
//: interface  /sz:(40, 40) /bd:[ ]
reg c0;    //: {0}(18:182,149)(288,149)(288,191){1}
//: {2}(290,193)(313,193){3}
//: {4}(288,195)(288,212){5}
reg a0;    //: {0}(40:182,185)(226,185){1}
//: {2}(230,185)(238,185){3}
//: {4}(228,187)(228,241)(245,241){5}
reg b0;    //: {0}(50:182,220)(220,220){1}
//: {2}(222,218)(222,190)(238,190){3}
//: {4}(222,222)(222,246)(245,246){5}
wire w7;    //: /sn:0 {0}(266,244)(281,244)(281,256){1}
wire s0;    //: {0}(36:334,191)(364,191){1}
wire w4;    //: /sn:0 {0}(286,233)(286,256){1}
wire c1;    //: {0}(99:284,277)(284,296){1}
wire w9;    //: /sn:0 {0}(313,188)(285,188){1}
//: {2}(281,188)(259,188){3}
//: {4}(283,190)(283,212){5}
//: enddecls

  //: LED g37 (s0) @(371,191) /sn:0 /R:3 /w:[ 1 ] /type:0
  //: comment g2 @(609,113) /sn:0 /anc:1
  //: /line:"<font color=green4>Hinweis: Klicke in eine Freifläche,"
  //: /line:"wenn eine Gruppe wieder"
  //: /line:"deselektiert werden soll."
  //: /line:""
  //: /end
  _GGNAND2 #(4) g1 (.I0(w7), .I1(w4), .Z(c1));   //: @(284,267) /sn:0 /R:3 /w:[ 1 1 0 ]
  _GGNAND2 #(4) g28 (.I0(a0), .I1(b0), .Z(w7));   //: @(256,244) /sn:0 /w:[ 5 5 0 ]
  //: joint g32 (w9) @(283, 188) /w:[ 1 -1 2 4 ]
  //: SWITCH g27 (c0) @(165,149) /sn:0 /w:[ 0 ] /st:0
  _GGNAND2 #(4) g31 (.I0(w9), .I1(c0), .Z(w4));   //: @(286,223) /sn:0 /R:3 /w:[ 5 5 0 ]
  //: joint g29 (a0) @(228, 185) /w:[ 2 -1 1 4 ]
  //: SWITCH g25 (a0) @(165,185) /sn:0 /w:[ 0 ] /st:0
  //: comment g5 @(10,10) /anc:1
  //: /line:"<h3>Editieren von Bausteingruppen</h3> <b>(Auswahl und Bewegung)</b>"
  //: /line:""
  //: /line:"Eine Gruppe von Gattern wird ausgewählt, indem die linke Maustaste links oberhalb der Gatter gedrückt und festgehalten wird und"
  //: /line:"dann der Mauszeiger diagonal nach rechts unten gezogen wird, um die Auswahlbox zu öffnen. Man gibt die Maus frei, wenn alle"
  //: /line:"gewünschten Gatter sich innerhalb der Box befinden. Jetzt kann die Gruppe an einem beliebigen Gatter angefaßt und bewegt werden."
  //: /line:"Versuche das bei der Schaltung unten."
  //: /end
  _GGXOR2 #(8) g24 (.I0(w9), .I1(c0), .Z(s0));   //: @(324,191) /sn:0 /w:[ 0 3 0 ]
  _GGXOR2 #(8) g23 (.I0(a0), .I1(b0), .Z(w9));   //: @(249,188) /sn:0 /w:[ 3 3 3 ]
  //: SWITCH g26 (b0) @(165,220) /sn:0 /w:[ 0 ] /st:0
  //: comment g0 @(10,310) /sn:0 /R:14 /anc:1
  //: /line:"<tutorial-navigation>"
  //: /end
  //: joint g33 (c0) @(288, 193) /w:[ 2 1 -1 4 ]
  //: joint g30 (b0) @(222, 220) /w:[ -1 2 1 4 ]

endmodule
//: /netlistEnd

//: /netlistBegin PAGE4
module PAGE4();
//: interface  /sz:(40, 40) /bd:[ ]
reg c0;    //: {0}(18:174,160)(280,160)(280,202){1}
//: {2}(282,204)(305,204){3}
//: {4}(280,206)(280,223){5}
reg a0;    //: {0}(40:174,196)(196,196)(196,196)(218,196){1}
//: {2}(222,196)(230,196){3}
//: {4}(220,198)(220,252)(237,252){5}
reg b0;    //: {0}(50:174,231)(193,231)(193,231)(212,231){1}
//: {2}(214,229)(214,201)(230,201){3}
//: {4}(214,233)(214,257)(237,257){5}
wire w7;    //: /sn:0 {0}(258,255)(273,255)(273,267){1}
wire s0;    //: {0}(36:326,202)(356,202){1}
wire w4;    //: /sn:0 {0}(278,244)(278,267){1}
wire c1;    //: {0}(99:276,288)(276,307){1}
wire w9;    //: /sn:0 {0}(305,199)(277,199){1}
//: {2}(273,199)(251,199){3}
//: {4}(275,201)(275,223){5}
//: enddecls

  //: LED g37 (s0) @(363,202) /sn:0 /R:3 /w:[ 1 ] /type:0
  //: comment g2 @(548,117) /sn:0 /anc:1
  //: /line:"<font color=green4>Hinweis: Durch Drücken der <font color=red2>Entf</font> Taste"
  //: /line:"können alle ausgewählten Gatter gelöscht"
  //: /line:"werden, ohne sie in die Zwischenablage"
  //: /line:"zu kopieren.</font>"
  //: /end
  _GGNAND2 #(4) g1 (.I0(w7), .I1(w4), .Z(c1));   //: @(276,278) /sn:0 /R:3 /w:[ 1 1 0 ]
  _GGNAND2 #(4) g28 (.I0(a0), .I1(b0), .Z(w7));   //: @(248,255) /sn:0 /w:[ 5 5 0 ]
  //: joint g32 (w9) @(275, 199) /w:[ 1 -1 2 4 ]
  //: SWITCH g27 (c0) @(157,160) /sn:0 /w:[ 0 ] /st:0
  _GGNAND2 #(4) g31 (.I0(w9), .I1(c0), .Z(w4));   //: @(278,234) /sn:0 /R:3 /w:[ 5 5 0 ]
  //: joint g29 (a0) @(220, 196) /w:[ 2 -1 1 4 ]
  //: SWITCH g25 (a0) @(157,196) /sn:0 /w:[ 0 ] /st:0
  //: comment g5 @(10,10) /anc:1
  //: /line:"<h3>Editieren von Bausteingruppen</h3> <b>(Zwischenablagefunktionen)</b>"
  //: /line:""
  //: /line:"Die Zwischenablage kann für das Ausschneiden oder Kopieren einer Gattergruppe genutzt werden, um sie nachher"
  //: /line:" woanders wieder einzufügen. Wähle eine Gruppe von Gattern aus klicke dann auf die <img src=edit_cut.gif bgcolor=gray> oder <img src=edit_copy.gif bgcolor=gray> Knöpfe, um sie "
  //: /line:"in die Zwischenablage auszuschneiden oder zu kopieren.  Zum Einfügen des Inhalts der Zwischenablage setzt man" 
  //: /line:"erst mit einem Linksklick ein <img src=mark.gif>, dann drückt man den <img src=edit_paste.gif bgcolor=gray> Knopf und der Inhalt der Zwischenablage erscheint an dieser Stelle."
  //: /line:""
  //: /end
  _GGXOR2 #(8) g24 (.I0(w9), .I1(c0), .Z(s0));   //: @(316,202) /sn:0 /w:[ 0 3 0 ]
  _GGXOR2 #(8) g23 (.I0(a0), .I1(b0), .Z(w9));   //: @(241,199) /sn:0 /w:[ 3 3 3 ]
  //: SWITCH g26 (b0) @(157,231) /sn:0 /w:[ 0 ] /st:0
  //: comment g0 @(10,310) /sn:0 /R:14 /anc:1
  //: /line:"<tutorial-navigation>"
  //: /end
  //: joint g33 (c0) @(280, 204) /w:[ 2 1 -1 4 ]
  //: joint g30 (b0) @(214, 231) /w:[ -1 2 1 4 ]

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

