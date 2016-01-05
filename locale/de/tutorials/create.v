//: version "2.0-b6"
//: property prefix = "_GG"
//: property title = "create_tut.v"
//: property useExtBars = 0
//: property discardChanges = 1
//: property encoding = "utf-8"
//: property locale = "de"

`timescale 1ns/1ns

//: /netlistBegin PAGE1
module PAGE1;    //: root_module
reg w3;    //: /sn:0 {0}(219,137)(154,137){1}
reg w9;    //: /sn:0 {0}(314,237)(213,237){1}
//: {2}(211,235)(211,142)(219,142){3}
//: {4}(209,237)(154,237){5}
wire w8;    //: /sn:0 {0}(314,232)(294,232)(294,140)(240,140){1}
wire w10;    //: /sn:0 {0}(335,235)(345,235){1}
//: enddecls

  //: comment g8 @(313,208)
  //: /line:"OR Gatter"
  //: /end
  //: joint g13 (w9) @(211, 237) /w:[ 1 2 4 -1 ]
  //: comment g1 @(10,310) /sn:0 /anc:1
  //: /line:"<tutorial-navigation>"
  //: /end
  _GGAND2 #(6) g11 (.I0(w3), .I1(w9), .Z(w8));   //: @(230,140) /sn:0 /w:[ 0 3 1 ]
  //: comment g6 @(115,104)
  //: /line:"Schalter"
  //: /end
  //: comment g9 @(10,10) /anc:1
  //: /line:"<h3>Erzeugen einer einfachen Schaltung</h3>"
  //: /line:""
  //: /line:"In diesem Abschnitt zeigen wir schrittweise, wie die einfache Schaltung unten erzeugt wird."
  //: /end
  //: comment g7 @(208,112)
  //: /line:"AND Gatter"
  //: /end
  //: SWITCH g14 (w3) @(137,137) /sn:0 /w:[ 1 ] /st:0
  _GGOR2 #(6) g0 (.I0(w8), .I1(w9), .Z(w10));   //: @(325,235) /sn:0 /w:[ 0 0 0 ]
  //: SWITCH g12 (w9) @(137,237) /sn:0 /w:[ 5 ] /st:0

endmodule
//: /netlistEnd

//: /netlistBegin PAGE6
module PAGE6();
//: interface  /sz:(40, 40) /bd:[ ]
reg w6;    //: /sn:0 {0}(154,237)(182,237)(182,237)(314,237){1}
reg w2;    //: /sn:0 {0}(219,137)(186,137)(186,137)(154,137){1}
wire w7;    //: /sn:0 {0}(335,235)(345,235){1}
wire w0;    //: /sn:0 {0}(204,142)(219,142){1}
wire w5;    //: /sn:0 {0}(314,232)(294,232)(294,140)(240,140){1}
//: enddecls

  _GGOR2 #(6) g4 (.I0(w5), .I1(w6), .Z(w7));   //: @(325,235) /sn:0 /w:[ 0 1 0 ]
  _GGAND2 #(6) g3 (.I0(w2), .I1(w0), .Z(w5));   //: @(230,140) /sn:0 /w:[ 0 1 1 ]
  //: SWITCH g2 (w6) @(137,237) /sn:0 /w:[ 0 ] /st:0
  //: comment g10 @(10,310) /sn:0 /R:14 /anc:1
  //: /line:"<tutorial-navigation>"
  //: /end
  //: comment g6 @(537,46) /sn:0 /anc:1
  //: /line:"<img src=\"makejoint_example.gif\">"
  //: /end
  //: comment g7 @(537,126) /sn:0 /anc:1
  //: /line:"<font color=green4>Hinweis: Verbindungen werden"
  //: /line:"immer dann erzeugt, wenn der Lötkolben"
  //: /line:"aktiv ist und die Leitung in der Mitte"
  //: /line:"einer anderen Leitung fallengelassen"
  //: /line:"wird.</font>"
  //: /end
  //: comment g9 @(10,10) /anc:1
  //: /line:"<h3>Erzeugen einer einfachen Schaltung</h3>"
  //: /line:""
  //: /line:"Erzeuge nun die letzte Verbindung.  Fasse den unteren Eingang des AND Gatters"
  //: /line:"und ziehe ihn in die Nähe des Drahtes zwischen dem unteren Schalter und dem OR Gatter."
  //: /line:"Lasse den Draht los, dann entsteht eine Lötverbindung."
  //: /line:""
  //: /end
  //: SWITCH g5 (w2) @(137,137) /sn:0 /w:[ 1 ] /st:0

endmodule
//: /netlistEnd

//: /netlistBegin PAGE3
module PAGE3();
//: interface  /sz:(40, 40) /bd:[ ]
wire w0;    //: /sn:0 {0}(204,137)(219,137){1}
wire w1;    //: /sn:0 {0}(204,142)(219,142){1}
wire w2;    //: /sn:0 {0}(240,140)(255,140){1}
//: enddecls

  _GGAND2 #(6) g2 (.I0(w0), .I1(w1), .Z(w2));   //: @(230,140) /sn:0 /w:[ 1 1 0 ]
  //: frame g1 @(283,202) /sn:0 /wi:78 /ht:46 /tx:""
  //: comment g10 @(10,310) /sn:0 /R:14 /anc:1
  //: /line:"<tutorial-navigation>"
  //: /end
  //: comment g9 @(10,10) /anc:1
  //: /line:"<h3>Erzeugen einer einfachen Schaltung</h3>"
  //: /line:""
  //: /line:"Erzeuge nun ein OR Gatter.  Klicke mit der rechten Maustaste in die strichlierte Box"
  //: /line:"und wähle <font color=red2>Bausteine &rarr;   Gatter &rarr;   OR</font> aus dem Popup Menu."
  //: /end
  //: comment g0 @(532,85) /sn:0 /anc:1
  //: /line:"<font color=green4>Hinweis: Genauso gut ist ein Tastendruck."
  //: /line:"Wähle die Position mit Linksklick und tippe"
  //: /line:"\"o\", um ein OR Gatter zu erzeugen.</font>"
  //: /end

endmodule
//: /netlistEnd

//: /netlistBegin PAGE5
module PAGE5();
//: interface  /sz:(40, 40) /bd:[ ]
reg w6;    //: /sn:0 {0}(154,237)(169,237){1}
reg w2;    //: /sn:0 {0}(154,137)(169,137){1}
wire w7;    //: /sn:0 {0}(240,140)(255,140){1}
wire w4;    //: /sn:0 {0}(204,137)(219,137){1}
wire w0;    //: /sn:0 {0}(299,232)(314,232){1}
wire w3;    //: /sn:0 {0}(335,235)(350,235){1}
wire w1;    //: /sn:0 {0}(299,237)(314,237){1}
wire w5;    //: /sn:0 {0}(204,142)(219,142){1}
//: enddecls

  //: SWITCH g4 (w2) @(137,137) /sn:0 /anc:1 /w:[ 0 ] /st:0
  _GGAND2 #(6) g3 (.I0(w4), .I1(w5), .Z(w7));   //: @(230,140) /sn:0 /anc:1 /w:[ 1 1 0 ]
  //: SWITCH g2 (w6) @(137,237) /sn:0 /anc:1 /w:[ 0 ] /st:0
  _GGOR2 #(6) g1 (.I0(w0), .I1(w1), .Z(w3));   //: @(325,235) /sn:0 /anc:1 /w:[ 1 1 0 ]
  //: comment g10 @(10,310) /sn:0 /R:14 /anc:1
  //: /line:"<tutorial-navigation>"
  //: /end
  //: comment g6 @(578,122) /sn:0 /anc:1
  //: /line:"<font color=green4>Hinweis: Der Lötkolben-Cursor zeigt"
  //: /line:"an, wenn ein verbindbares Drahtende" 
  //: /line:"ausgewählt ist.</font>"
  //: /end
  //: comment g9 @(10,10) /anc:1
  //: /line:"<h3>Erzeugen einer einfachen Schaltung</h3>"
  //: /line:""
  //: /line:"Verbinde schließlich die Leitungen.  Fasse ein Schalter-Drahtende mit der linken Maustaste"
  //: /line:"und ziehe es zum oberen Eingang des AND Gatters."
  //: /end
  //: comment g0 @(579,61) /sn:0 /anc:1
  //: /line:"<img src=\"connect_example.gif\">"
  //: /end

endmodule
//: /netlistEnd

//: /netlistBegin PAGE4
module PAGE4();
//: interface  /sz:(40, 40) /bd:[ ]
wire w7;    //: /sn:0 {0}(335,235)(350,235){1}
wire w4;    //: /sn:0 {0}(299,232)(314,232){1}
wire w3;    //: /sn:0 {0}(240,140)(255,140){1}
wire w0;    //: /sn:0 {0}(204,137)(219,137){1}
wire w1;    //: /sn:0 {0}(204,142)(219,142){1}
wire w5;    //: /sn:0 {0}(299,237)(314,237){1}
//: enddecls

  //: frame g8 @(109,215) /sn:0 /anc:1 /wi:76 /ht:43 /tx:""
  _GGOR2 #(6) g4 (.I0(w4), .I1(w5), .Z(w7));   //: @(325,235) /sn:0 /anc:1 /w:[ 1 1 0 ]
  _GGAND2 #(6) g3 (.I0(w0), .I1(w1), .Z(w3));   //: @(230,140) /sn:0 /anc:1 /w:[ 1 1 0 ]
  //: frame g1 @(109,121) /sn:0 /anc:1 /wi:76 /ht:43 /tx:""
  //: comment g10 @(10,310) /sn:0 /R:14 /anc:1
  //: /line:"<tutorial-navigation>"
  //: /end
  //: comment g9 @(10,10) /anc:1
  //: /line:"<h3>Erzeugen einer einfachen Schaltung</h3>"
  //: /line:""
  //: /line:"Erzuge als nächstes zwei Schalter.  Klicke rechts in jede der strichlierten Boxen"
  //: /line:"und wähle <font color=red2>Bausteine &rarr;   I/O &rarr;   Schalter</font> aus dem Popup Menu."
  //: /end
  //: comment g0 @(532,86) /sn:0 /anc:1
  //: /line:"<font color=green4>Hinweis: Genauso gut ist ein Tastendruck."
  //: /line:"Wähle die Position mit Linksklick und tippe"
  //: /line:"\"s\", um einen Schalter zu erzeugen.</font>"
  //: /end

endmodule
//: /netlistEnd

//: /netlistBegin PAGE2
module PAGE2();
//: interface  /sz:(40, 40) /bd:[ ]
//: enddecls

  //: comment g2 @(531,86) /sn:0 /anc:1
  //: /line:"<font color=green4>Hinweis: Genauso gut ist ein Tastendruck."
  //: /line:"Wähle die Position mit Linksklick und tippe"
  //: /line:"\"a\", um ein AND Gatter zu erzeugen.</font>"
  //: /end
  //: frame g1 @(208,124) /sn:0 /wi:76 /ht:43 /tx:""
  //: comment g9 @(10,10) /anc:1
  //: /line:"<h3>Erzeugen einer einfachen Schaltung</h3>"
  //: /line:""
  //: /line:"Beginne mit der Erzeugung eines AND Gatters.  Klicke mit rechts in die strichlierte Box und wähle"
  //: /line:"<font color=red2>Bausteine &rarr;   Gatter &rarr;   AND</font> aus dem Popup Menu."
  //: /end
  //: comment g0 @(10,310) /sn:0 /R:14 /anc:1
  //: /line:"<tutorial-navigation>"
  //: /end

endmodule
//: /netlistEnd


`timescale 1ns/1ns


//: /builtinBegin
module _GGAND2 #(.Diz(1)) (I0, I1, Z);
output Z;
input I0;
input I1;

  assign #Diz Z = I0 & I1;

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

