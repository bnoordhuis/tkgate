//: version "2.0-b6"
//: property prefix = "_GG"
//: property title = "module_tut.v"
//: property discardChanges = 1
//: property encoding= "utf-8"
//: property locale = "de"

`timescale 1ns/1ns

//: /netlistBegin PAGE1
module PAGE1;    //: root_module
//: enddecls

  //: comment g3 @(259,83) /anc:1
  //: /line:"Ein Modul ist ein gekapselte Stück Schaltungslogik, das in einer"
  //: /line:"größeren Schaltung eine bestimmte Aufgabe erfüllt, ganz ähnlich"
  //: /line:"wie ein Modul oder eine Klasse in einem Programm."
  //: /line:"Jedes Modul in TkGate hat eine \"Schnittstelle\" und"
  //: /line:"eine  \"Definition\":"
  //: /line:""
  //: /line:"  *  Die <i color=green4>Schnittstelle</i> beschreibt die äußere Erscheinung"
  //: /line:"     eines Moduls einschließlich der Größe der Box und der"
  //: /line:"     Positionen der Ports auf ihr. TkGate unterstützt \"block\""
  //: /line:"     Schnittstellen und \"symbolische\" Schnittstellen."
  //: /line:"  "
  //: /line:"  *  Die <i color=green4>Definition</i> beschreibt die interne, durch das Modul"
  //: /line:"     implementierte Logik. TkGate unterstützt \"Netzlisten\""
  //: /line:"     und \"HDL\" Module."
  //: /line:""
  //: /end
  //: comment g2 @(10,65) /sn:0 /anc:1
  //: /line:"<img src=bigmodule.gif>"
  //: /end
  //: comment g1 @(9,13) /anc:1
  //: /line:"<h3>Module benutzen</h3>"
  //: /line:""
  //: /line:"In diesem Abschnitt wird gezeigt, wie man Module erzeugt, editiert und verwaltet."
  //: /end
  //: comment g0 @(10,310) /sn:0 /R:14 /anc:1
  //: /line:"<tutorial-navigation>"
  //: /end

endmodule
//: /netlistEnd

//: /netlistBegin PAGE6
module PAGE6();
//: interface  /sz:(40, 40) /bd:[ ]
wire w4;    //: /sn:0 {0}(258,177)(273,177){1}
wire w0;    //: /sn:0 {0}(200,161)(185,161){1}
wire w3;    //: /sn:0 {0}(258,161)(273,161){1}
wire w1;    //: /sn:0 {0}(200,177)(185,177){1}
wire w2;    //: /sn:0 {0}(200,192)(185,192){1}
wire w5;    //: /sn:0 {0}(258,193)(273,193){1}
//: enddecls

  bar g4 (.C(w2), .B(w1), .A(w0), .Z(w5), .Y(w4), .X(w3));   //: @(201, 145) /sz:(56, 72) /sn:0 /p:[ Li0>0 Li1>0 Li2>0 Ro0<0 Ro1<0 Ro2<0 ]
  //: comment g3 @(10,310) /sn:0 /R:14
  //: /line:"<tutorial-navigation>"
  //: /end
  //: comment g2 @(10,310) /sn:0 /R:14
  //: /line:"<tutorial-navigation>"
  //: /end
  //: comment g1 @(9,13) /anc:1
  //: /line:"<h3>Module benutzen</h3> <b>(Editieren von Moduldefinitionen)</b>"
  //: /line:""
  //: /line:"Ein Doppelklick auf den Namen eines selbsterzeugten Moduls in der Modulbaum- oder Modullistenansicht"
  //: /line:"ermöglicht das Editieren der Moduldefinition. Alternativ dazu kann man auch auf eine Modulinstanz rechtsklicken"
  //: /line:"und <font color=red2>Öffnen</font> aus dem Popup Menu wählen."
  //: /end
  //: comment g0 @(10,310) /sn:0 /R:14
  //: /line:"<tutorial-navigation>"
  //: /end

endmodule
//: /netlistEnd

//: /netlistBegin PAGE3
module PAGE3();
//: interface  /sz:(40, 40) /bd:[ ]
//: enddecls

  //: comment g4 @(576,89) /sn:0 /anc:1
  //: /line:"<font color=green4>Hinweis: Die Seiten dieses Tutoriums"
  //: /line:"sind in Wirklichkeit Module mit den"
  //: /line:"Namen PAGE1, PAGE2, etc.</font>"
  //: /end
  //: comment g3 @(23,72) /sn:0 /anc:1
  //: /line:"<img src=document.gif>   Oberstes oder Wurzelmodul."
  //: /line:""
  //: /line:"<img src=mod_net.gif>   Netzlistenmodul."
  //: /line:""
  //: /line:"<img src=mod_hdl.gif>   Verilog HDL Modul."
  //: /line:""
  //: /line:"<img src=mod_netL.gif>   Verriegeltes Netzlistenmodul."
  //: /line:""
  //: /line:"<img src=mod_hdlL.gif>   Verriegeltes Verilog HDL Modul."
  //: /line:""
  //: /line:"<img src=unused.gif>   Behälter für unbenutzte Module."
  //: /line:""
  //: /end
  //: comment g2 @(257,69) /anc:1
  //: /line:"<img src=chip.gif>   Bibliotheksmodul."
  //: /line:""
  //: /line:"<img src=parts.gif>   Behälter für alle Bibliotheken."
  //: /line:""
  //: /line:"<img src=ichipdir.gif>   Behälter für eine einzige Bibliothek."
  //: /line:""
  //: /line:"<img src=unknown.gif>   Benutzung eines undefinierten Moduls."
  //: /line:""
  //: /line:"<img src=conflict.gif>   Rekursive Einbindung eines Moduls."
  //: /line:""
  //: /end
  //: comment g1 @(9,13) /anc:1
  //: /line:"<h3>Module benutzen</h3> <b>(Modullistensymbole)</b>"
  //: /line:""
  //: /line:"Folgende Symbole werden in der Baum- oder Listenansicht der Module verwendet:"
  //: /line:""
  //: /end
  //: comment g0 @(10,310) /sn:0 /R:14 /anc:1
  //: /line:"<tutorial-navigation>"
  //: /end

endmodule
//: /netlistEnd

//: /netlistBegin ALU
module ALU(F, A, B, Z);
//: interface  /sz:(40, 40) /bd:[ ]
input [7:0] B;    //: /sn:0 {0}(#:342,147)(342,184){1}
//: {2}(344,186)(367,186){3}
//: {4}(371,186)(425,186)(425,258){5}
//: {6}(369,188)(369,261){7}
//: {8}(342,188)(342,258){9}
input [1:0] F;    //: /sn:0 {0}(#:343,353)(214,353){1}
input [7:0] A;    //: /sn:0 {0}(#:265,147)(265,215){1}
//: {2}(267,217)(345,217){3}
//: {4}(349,217)(372,217){5}
//: {6}(376,217)(457,217)(457,258){7}
//: {8}(374,219)(374,261){9}
//: {10}(347,219)(347,258){11}
//: {12}(265,219)(265,322)(348,322)(348,337){13}
supply0 w0;    //: /sn:0 {0}(491,295)(491,272)(465,272){1}
output [7:0] Z;    //: /sn:0 {0}(#:366,404)(366,366){1}
wire [7:0] w14;    //: /sn:0 {0}(#:345,279)(345,310)(360,310)(360,337){1}
wire w4;    //: /sn:0 {0}(417,272)(407,272){1}
wire [7:0] w11;    //: /sn:0 {0}(#:372,282)(372,337){1}
wire [7:0] w10;    //: /sn:0 {0}(#:384,337)(384,307)(441,307)(441,287){1}
//: enddecls

  //: joint g8 (A) @(347, 217) /w:[ 4 -1 3 10 ]
  _GGADD8 #(68, 70, 62, 64) g4 (.A(B), .B(A), .S(w10), .CI(w0), .CO(w4));   //: @(441,274) /sn:0 /w:[ 5 7 1 1 0 ]
  //: GROUND g13 (w0) @(491,301) /sn:0 /w:[ 0 ]
  //: IN g3 (F) @(212,353) /sn:0 /w:[ 1 ]
  //: IN g2 (A) @(265,145) /sn:0 /R:3 /w:[ 0 ]
  //: IN g1 (B) @(342,145) /sn:0 /R:3 /w:[ 0 ]
  //: joint g11 (B) @(369, 186) /w:[ 4 -1 3 6 ]
  //: joint g10 (A) @(374, 217) /w:[ 6 -1 5 8 ]
  _GGAND2x8 #(6) g6 (.I0(B), .I1(A), .Z(w11));   //: @(372,272) /sn:0 /R:3 /w:[ 7 9 0 ] /eb:0
  //: joint g9 (A) @(265, 217) /w:[ 2 1 -1 12 ]
  _GGOR2x8 #(6) g7 (.I0(B), .I1(A), .Z(w14));   //: @(345,269) /sn:0 /R:3 /w:[ 9 11 0 ] /eb:0
  //: OUT g5 (Z) @(366,401) /sn:0 /R:3 /w:[ 0 ]
  _GGMUX4x8 #(12, 12) g0 (.I0(A), .I1(w14), .I2(w11), .I3(w10), .S(F), .Z(Z));   //: @(366,353) /sn:0 /w:[ 13 1 1 0 0 1 ] /ss:0 /do:0
  //: joint g12 (B) @(342, 186) /w:[ 2 1 -1 8 ]

endmodule
//: /netlistEnd

//: /netlistBegin foo
module foo(Z2, I3, Z1, I1, I2, Z3);
//: interface  /sz:(41, 40) /bd:[ ]
input I2;    //: /sn:0 {0}(245,200)(275,200){1}
//: {2}(277,198)(277,153)(292,153){3}
//: {4}(277,202)(277,248)(348,248){5}
output Z3;    //: /sn:0 {0}(369,251)(448,251){1}
input I1;    //: /sn:0 {0}(292,148)(278,148)(278,128)(246,128){1}
output Z2;    //: /sn:0 {0}(392,156)(380,156)(380,184){1}
//: {2}(382,186)(448,186){3}
//: {4}(378,186)(369,186){5}
output Z1;    //: /sn:0 {0}(448,154)(413,154){1}
input I3;    //: /sn:0 {0}(348,188)(296,188)(296,251){1}
//: {2}(298,253)(348,253){3}
//: {4}(294,253)(245,253){5}
wire w0;    //: /sn:0 {0}(348,183)(338,183)(338,153){1}
//: {2}(340,151)(392,151){3}
//: {4}(336,151)(313,151){5}
//: enddecls

  //: OUT g4 (Z2) @(445,186) /sn:0 /w:[ 3 ]
  _GGAND2 #(6) g8 (.I0(I2), .I1(I3), .Z(Z3));   //: @(359,251) /sn:0 /w:[ 5 3 0 ] /eb:0
  //: joint g13 (w0) @(338, 151) /w:[ 2 -1 4 1 ]
  //: IN g2 (I3) @(243,253) /sn:0 /w:[ 5 ]
  //: OUT g1 (Z1) @(445,154) /sn:0 /w:[ 0 ]
  _GGAND2 #(6) g11 (.I0(w0), .I1(Z2), .Z(Z1));   //: @(403,154) /sn:0 /w:[ 3 0 1 ] /eb:0
  //: comment g16 @(585,127) /sn:0
  //: /line:"<font color=green4>Hinweis: Wurde das Modul durch Auswahl"
  //: /line:"von <img src=blk_open.gif><font color=red2>Öffnen</font> aus dem Menu oder der"
  //: /line:"Toolbar geöffnet, kann es durch Rechtsklick"
  //: /line:"in die Planfläche mit Auswahl von"
  //: /line:"<font color=red2>Schließen</font> wieder geschlossen werden."
  //: /end
  //: joint g10 (I2) @(277, 200) /w:[ -1 2 1 4 ]
  //: IN g19 (I1) @(244,128) /sn:0 /w:[ 1 ]
  _GGXOR2 #(8) g6 (.I0(I1), .I1(I2), .Z(w0));   //: @(303,151) /sn:0 /w:[ 0 3 5 ] /eb:0
  _GGXOR2 #(8) g7 (.I0(w0), .I1(I3), .Z(Z2));   //: @(359,186) /sn:0 /w:[ 0 0 5 ] /eb:0
  //: joint g9 (I3) @(296, 253) /w:[ 2 1 4 -1 ]
  //: comment g15 @(22,25)
  //: /line:"Dies ist die Definition des Moduls \"foo\"."
  //: /end
  //: IN g5 (I2) @(243,200) /sn:0 /w:[ 0 ]
  //: comment g14 @(76,306) /sn:0
  //: /line:"<a href=\"#/PAGE8\">&lang;Zurück zum Tutorium&rang;</a>"
  //: /end
  //: joint g23 (Z2) @(380, 186) /w:[ 2 1 4 -1 ]
  //: OUT g12 (Z3) @(445,251) /sn:0 /w:[ 1 ]

endmodule
//: /netlistEnd

//: /netlistBegin PAGE8
module PAGE8();
//: interface  /sz:(40, 40) /bd:[ ]
//: enddecls

  //: comment g3 @(500,160) /sn:0 /anc:1
  //: /line:"<font color=green4>Hinweis: Falls die Änderung einer Modulschnittstelle"
  //: /line:"nicht gelingt, klicke rechts auf sie und"
  //: /line:"wähle <img src=i_modprops.gif></font><font color=red2>Moduleigenschaften...</font><font color=green4> und setze"
  //: /line:"dann die Schutzeigenschaften der Schnittstelle"
  //: /line:"in der aufkommenden Dialogbox.</font>"
  //: /end
  foo g2 ();   //: @(208, 163) /sz:(40, 40) /sn:0 /p:[ ]
  //: comment g1 @(9,13) /anc:1
  //: /line:"<h3>Module benutzen</h3> <b>(Schnittstellenerzeugung von Hand)</b>"
  //: /line:""
  //: /line:"Die Schnittstelle eines Moduls kann auch von Hand erzeugt und editiert werden:"
  //: /line:" * Mit Linksklick auf eine Kante oder Ecke und Ziehen wird die Größe der Schnittstelle verändert."
  //: /line:" * Mit einem Rechtsklick auf eine Kante kann der zu erzeugende Porttyp aus dem Popup Menu gewählt werden."
  //: /line:" * Um einen Port wieder zu löschen, wählt man das <img src=cut_curs.gif> Werkzeug und schneidet die Leitung am Modul ab."
  //: /line:" * Mit einem Doppelklick auf einem Port mit dem <img src=mov_curs.gif> Werkzeug können die Porteigenschaften editiert werden."
  //: /end
  //: comment g0 @(10,310) /sn:0 /R:14
  //: /line:"<tutorial-navigation>"
  //: /end

endmodule
//: /netlistEnd

//: /netlistBegin PAGE9
module PAGE9();
//: interface  /sz:(40, 40) /bd:[ ]
wire [7:0] w3;    //: /sn:0 {0}(#:329,194)(314,194){1}
wire [1:0] w0;    //: /sn:0 {0}(#:257,226)(272,226){1}
wire [7:0] w1;    //: /sn:0 {0}(#:257,210)(272,210){1}
wire [7:0] w2;    //: /sn:0 {0}(#:257,194)(272,194){1}
//: enddecls

  ALU g3 (.A(w2), .B(w1), .F(w0), .Z(w3));   //: @(273, 178) /sz:(40, 64) /sn:0 /p:[ Li0>1 Li1>1 Li2>1 Ro0<1 ]
  //: comment g2 @(10,310) /sn:0 /R:14
  //: /line:"<tutorial-navigation>"
  //: /end
  //: comment g1 @(9,13) /anc:1
  //: /line:"<h3>Module benutzen</h3> <b>(Standardschnittstellen aktualisieren)</b>"
  //: /line:""
  //: /line:"Modulschnittstellen, die im <img src=editmode.gif> <font color=red2>Edit</font> Modus erzeugt wurden, gehören nur zur speziellen Instanz, aus der sie geschaffen wurden."
  //: /line:"Um eine Schnittstelle zur Standardschnittstelle für neu erzeugte Modulinstanzen dieses Typs zu machen, klickt man mit rechts"
  //: /line:"auf eine Schnittstelle und wählt <font color=red2>Schnittstelle &rarr;  Setzen</font>.  Versuche die Schnittstellen von jeder der beiden Instanzen"
  //: /line:"der ALU Module unten zu setzen. Versuche dann, neue ALU Module durch Ziehen aus der Modulliste zu erzeugen."
  //: /end
  ALU g0 ();   //: @(130, 184) /sz:(40, 40) /sn:0 /p:[ ]

endmodule
//: /netlistEnd

//: /netlistBegin PAGE7
module PAGE7();
//: interface  /sz:(40, 40) /bd:[ ]
//: enddecls

  //: comment g1 @(9,13) /anc:1
  //: /line:"<h3>Module benutzen</h3> <b>(Automatische Schnittstellenerzeugung)</b>"
  //: /line:""
  //: /line:"Die Schnittstelle eines Moduls enthält Größe und Form der Box und Ort und Namen der Ports:"
  //: /line:"Am einfachsten ist es, die Schnittstelle automatisch generieren zu lassen.  Automatische"
  //: /line:"Erzeugung nutzt die in der Definition festgelegten Ports zur Generierung der Schnittstelle."
  //: /line:"Klicke auf den Modullisten-Tab <img src=modlist.gif bgcolor=gray>, um automatisch eine Schnittstelle für das \"bar\" Modul"
  //: /line:"zu erzeugen. Klicke mit rechts auf \"bar\" und wähle <font color=red2>Schnittstelle&rarr; Automatisch erzeugen...</font>."  
  //: /line:"Dann ziehe mit einem Linksklick das \"bar\" Modul auf die Planfläche, um die neue Schnittstelle"
  //: /line:"zu sehen."
  //: /line:""
  //: /end
  //: comment g0 @(10,310) /sn:0 /R:14
  //: /line:"<tutorial-navigation>"
  //: /end

endmodule
//: /netlistEnd

//: /netlistBegin PAGE5
module PAGE5();
//: interface  /sz:(40, 40) /bd:[ ]
//: enddecls

  //: comment g1 @(9,13) /anc:1
  //: /line:"<h3>Module benutzen</h3> <b>(Moduldefinitionen erzeugen)</b>"
  //: /line:""
  //: /line:"Zum Erzeugen eines neuen Moduls drückt man den <img src=blk_new.gif> Knopf. Gib den Namen für das neue Modul ein"
  //: /line:"und wähle, ob es ein graphisches Netzlistenmodul oder ein textuelles HDL Modul werden soll."
  //: /line:"Versuche, ein neues Modul namens \"CPU\" zu erzeugen."
  //: /end
  //: comment g0 @(10,310) /sn:0 /R:14
  //: /line:"<tutorial-navigation>"
  //: /end

endmodule
//: /netlistEnd

//: /netlistBegin PAGE4
module PAGE4();
//: interface  /sz:(40, 40) /bd:[ ]
//: enddecls

  //: comment g2 @(545,102) /sn:0 /anc:1
  //: /line:"<font color=green4>Hinweis: Man kann auch eine Modulinstanz"
  //: /line:"aus der Baumansicht ziehen und plazieren,"
  //: /line:"es ist aber leichter mit der Listenansicht,"
  //: /line:"weil es da eine alphabetische Sicht"
  //: /line:"auf alle Module gibt.</font>"
  //: /end
  //: comment g1 @(9,13) /anc:1
  //: /line:"<h3>Module benutzen</h3> <b>(Modulinstanzen erzeugen)</b>"
  //: /line:""
  //: /line:"Am leichtesten läßt sich eine Modulinstanz erzeugen, wenn man den Modulnamen in der Modulliste findet"
  //: /line:"und ihn in die aktuelle Schaltung hineinzieht. Versuche, durch Umschalten mit <img src=modlist.gif> auf die Listenansicht"
  //: /line:"einzustellen und ziehe mehere Instanzen des \"bar\" Moduls ins aktuelle Editierfenster."
  //: /end
  //: comment g0 @(10,310) /sn:0 /R:14 /anc:1
  //: /line:"<tutorial-navigation>"
  //: /end

endmodule
//: /netlistEnd

//: /netlistBegin PAGE2
module PAGE2();
//: interface  /sz:(40, 40) /bd:[ ]
//: enddecls

  //: comment g3 @(620,6) /sn:0 /anc:1
  //: /line:"<img src=modlistfig.gif>"
  //: /end
  //: comment g2 @(620,346) /sn:0 /anc:1
  //: /line:"<font color=green4>Beispiel einer Modulbaumansicht</font>"
  //: /end
  //: comment g1 @(9,13) /anc:1
  //: /line:"<h3>Module benutzen</h3> <b>(Modulliste)</b>"
  //: /line:""
  //: /line:"Die Menge der zur aktuellen Schaltung gehörenden Module wird auf der linken Seite der "
  //: /line:"Schnittstelle gezeigt.  Man kann zwischen einer <i>Baumansicht</i> <img src=modtree.gif bgcolor=gray> (siehe Beispiel) und einer"
  //: /line:"<i>Listenansicht</i> <img src=modlist.gif bgcolor=gray> entscheiden.  Die Baumansicht zeigt die Modulhierarchie, beginnend mit"
  //: /line:"einem obersten Wurzelmodul, das durch <img src=document.gif> symbolisiert wird. Module, die noch zur Schaltung"
  //: /line:"gehören, aber nirgends mehr in der Hierarchie benutzt werden, sind im \"<img src=unused.gif> Unbenutzten\""
  //: /line:"Zweig gelistet.  Klicke auf <img src=hboxopen.gif>, um einen Zweig im Baum zu öffnen."
  //: /line:""
  //: /end
  //: comment g0 @(10,310) /sn:0 /R:14 /anc:1
  //: /line:"<tutorial-navigation>"
  //: /end

endmodule
//: /netlistEnd

//: /netlistBegin PAGE10
module PAGE10();
//: interface  /sz:(40, 40) /bd:[ ]
//: enddecls

  //: comment g2 @(10,310) /sn:0 /R:14
  //: /line:"<tutorial-navigation>"
  //: /end
  //: comment g1 @(11,22) /anc:1
  //: /line:"<h3>Module benutzen</h3> <b>(Schnittstelleneditor)</b>"
  //: /line:""
  //: /line:"Die Modulschnittstelle kann direkt mit TkGate's Schnittstellenmodus editiert werden. Klicke auf das <img src=editintr.gif> <font color=red2>Schnittstelle</font>n Tab"
  //: /line:"über dem dem Haupteditierfeld, um in diesen Modus zu gelangen. Details zu Benutzung dieses Modus stehen in der"
  //: /line:"TkGate Dokumentation.  Klicke auf den <img src=editmode.gif> <font color=red2>Edit</font> Tab, um zum normalen Editiermodus zurückzukehren."
  //: /end

endmodule
//: /netlistEnd

//: /netlistBegin bar
module bar(Y, X, C, B, Z, A);
//: interface  /sz:(56, 72) /bd:[ Li0>C(48/72) Li1>B(32/72) Li2>A(16/72) Ro0<Z(48/72) Ro1<Y(32/72) Ro2<X(16/72) ]
//: property pptype=0
input B;    //: /sn:0 {0}(86,207)(116,207){1}
//: {2}(118,205)(118,160)(133,160){3}
//: {4}(118,209)(118,255)(189,255){5}
output X;    //: /sn:0 {0}(346,161)(268,161){1}
//: {2}(266,159)(266,131)(282,131){3}
//: {4}(264,161)(254,161){5}
input A;    //: /sn:0 {0}(133,155)(50:118,155)(118,135)(86,135){1}
output Z;    //: /sn:0 {0}(210,258)(346,258){1}
input C;    //: /sn:0 {0}(189,195)(137,195)(137,258){1}
//: {2}(139,260)(160,260){3}
//: {4}(164,260)(189,260){5}
//: {6}(162,258)(162,126)(282,126){7}
//: {8}(135,260)(86,260){9}
output Y;    //: /sn:0 {0}(233,163)(221,163)(221,191){1}
//: {2}(223,193)(346,193){3}
//: {4}(219,193)(210,193){5}
wire w0;    //: /sn:0 {0}(189,190)(179,190)(179,160){1}
//: {2}(181,158)(233,158){3}
//: {4}(177,158)(154,158){5}
wire w3;    //: /sn:0 {0}(303,129)(318,129){1}
//: enddecls

  //: OUT g4 (Y) @(343,193) /sn:0 /w:[ 3 ]
  _GGAND2 #(6) g8 (.I0(B), .I1(C), .Z(Z));   //: @(200,258) /sn:0 /w:[ 5 5 0 ] /eb:0
  //: joint g13 (w0) @(179, 158) /w:[ 2 -1 4 1 ]
  //: OUT g3 (X) @(343,161) /sn:0 /w:[ 0 ]
  //: IN g2 (C) @(84,260) /sn:0 /w:[ 9 ]
  //: IN g1 (B) @(84,207) /sn:0 /w:[ 0 ]
  //: comment g16 @(585,127) /sn:0 /anc:1
  //: /line:"<font color=green4>Hinweis: Falls dieses Modul durch Anwahl von"
  //: /line:"<img src=blk_open.gif><font color=red2>Öffnen</font> aus Menu oder"
  //: /line:"Toolbar geöffnet wurde, kann es durch einen Rechtsklick"
  //: /line:"in das Editierfeld und die Anwahl von"
  //: /line:"<font color=red2>Schließen</font> geschlossen werden."
  //: /end
  _GGAND2 #(6) g11 (.I0(w0), .I1(Y), .Z(X));   //: @(244,161) /sn:0 /w:[ 3 0 5 ] /eb:0
  //: joint g10 (B) @(118, 207) /w:[ -1 2 1 4 ]
  //: joint g19 (C) @(162, 260) /w:[ 4 6 3 -1 ]
  _GGXOR2 #(8) g6 (.I0(A), .I1(B), .Z(w0));   //: @(144,158) /sn:0 /w:[ 0 3 5 ] /eb:0
  _GGXOR2 #(8) g7 (.I0(w0), .I1(C), .Z(Y));   //: @(200,193) /sn:0 /w:[ 0 0 5 ] /eb:0
  //: joint g9 (C) @(137, 260) /w:[ 2 1 8 -1 ]
  //: comment g15 @(22,25)
  //: /line:"Dies ist die Definiton für das Modul \"bar\". Ports A, B und C sind Eingänge und Ports X, Y und Z sind"
  //: /line:"Ausgänge.  Versuche, einen zusätzlichen Ausgangsport \"W\" aus dem Ausgang des XOR Gatters unten zu erzeugen."
  //: /line:"Durch Rechtsklick an der Position, wo der Port erzeugt werden soll, und Anwahl von <font color=red2>Bausteine &rarr;   Module &rarr;   Modulausgang</font>"
  //: /line:"aus dem Popup Menu.  Gib als Netznamen \"W\" in die Dialog box ein."
  //: /line:""
  //: /line:""
  //: /end
  _GGXOR2 #(8) g17 (.I0(C), .I1(X), .Z(w3));   //: @(293,129) /sn:0 /w:[ 7 3 0 ]
  //: comment g14 @(76,306) /sn:0
  //: /line:"<a href=\"#/PAGE6\">&lang;BACK TO TUTORIAL&rang;</a>"
  //: /end
  //: OUT g5 (Z) @(343,258) /sn:0 /w:[ 1 ]
  //: IN g0 (A) @(84,135) /sn:0 /w:[ 1 ]
  //: joint g12 (Y) @(221, 193) /w:[ 2 1 4 -1 ]
  //: joint g18 (X) @(266, 161) /w:[ 1 2 4 -1 ]

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
module _GGMUX4x8 #(.Dsz(1), .Diz(1)) (I0, I1, I2, I3, S, Z);
input [7:0] I0, I1, I2, I3;
input [1:0] S;
output [7:0] Z;
reg [7:0] Z;

  always @(*)
    begin
      case (S)
          2'h0: Z <= #Dsz I0;
          2'h1: Z <= #Dsz I1;
          2'h2: Z <= #Dsz I2;
          2'h3: Z <= #Dsz I3;
        default: Z <= #Dsz  2'h0;
      endcase
    end

endmodule
//: /builtinEnd


//: /builtinBegin
module _GGOR2x8 #(.Diz(1)) (I0, I1, Z);
output [7:0] Z;
input [7:0] I0;
input [7:0] I1;

  assign #Diz Z = I0 | I1;

endmodule
//: /builtinEnd


//: /builtinBegin
module _GGADD8 #(.Dab_s(1), .Dab_co(1), .Dci_s(1), .Dci_co(1)) (A, B, S, CI, CO);
input CI;
output CO;
input [7:0] A,B;
output [7:0] S;

  specify
    (A,B *> S) = Dab_s;
    (A,B *> CO) = Dab_co;
    (CI *> S) = Dci_s;
    (CI *> CO) = Dci_co;
  endspecify

  assign {CO,S} = A + B + CI;

endmodule
//: /builtinEnd


//: /builtinBegin
module _GGAND2x8 #(.Diz(1)) (I0, I1, Z);
output [7:0] Z;
input [7:0] I0;
input [7:0] I1;

  assign #Diz Z = I0 & I1;

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

