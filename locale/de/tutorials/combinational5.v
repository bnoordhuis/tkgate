//: version "2.0-b6"
//: property encoding = "utf-8"
//: property locale = "de"
//: property prefix = "_GG"
//: property title = "sim_tut.v"
//: property useExtBars = 0
//: property discardChanges = 1

`timescale 1ns/1ns

//: /netlistBegin PAGE1
module PAGE1;    //: root_module
reg [3:0] w6;    //: /sn:0 {0}(#:171,282)(201,282)(201,262)(244,262){1}
reg w1;    //: /sn:0 {0}(279,220)(279,186){1}
reg [3:0] w5;    //: /sn:0 {0}(#:168,209)(203,209)(203,237)(244,237){1}
wire [3:0] w4;    //: /sn:0 {0}(#:310,248)(357,248)(357,228){1}
wire w0;    //: /sn:0 {0}(277,319)(277,286){1}
//: enddecls

  //: LED g4 (w4) @(357,221) /sn:0 /w:[ 1 ] /type:1
  //: SWITCH g3 (w1) @(279,173) /sn:0 /R:3 /w:[ 1 ] /st:0
  //: comment g13 @(10,10) /anc:1
  //: /line:"<h3>Simulation von Schaltnetzen</h3> <b>(Instanzhierarchie)</b>"
  //: /line:""
  //: /line:"Während der Simulator aktiv ist, zeigt die hierarchische Modulliste die Hierarchie der Instanzen"
  //: /line:"anstelle der Module an.  Denn der Zustand (die Netzeigenschaften) jeder Instanz eines Moduls in der"
  //: /line:"Hierarchie können verschieden sein. Rufe die simulation der untenstehenden Schaltung auf und entlasse"
  //: /line:"sie aus der Pause. Untersuche dann die Netze in verschiedenen Instanzen des ADD1-Moduls. Klicke jedesmal"
  //: /line:"doppelt auf \"g0&lt;ADD1>\",\"g1&lt;ADD1>\", etc. in der Hierarchielisten-Box, um zu jeder Instanz zu gelangen."
  //: /line:""
  //: /end
  //: DIP g2 (w6) @(133,282) /sn:0 /R:1 /w:[ 0 ] /st:15
  //: DIP g1 (w5) @(130,209) /sn:0 /R:1 /w:[ 0 ] /st:3
  //: comment g6 @(540,147) /sn:0 /anc:1
  //: /line:"<font color=green4>Hinweis, zur Erinnerung: Start und Rückkehr"
  //: /line:"aus der Pause geht durch Drücken der"
  //: /line:"\"<img src=\"simstart.gif\"> <font color=red2>Simulieren</font>\" Taste,"
  //: /line:"gefolgt vom  <img src=sim_go.gif> Knopf."
  //: /end
  //: comment g7 @(540,227) /sn:0 /anc:1
  //: /line:"<font color=green4>Hinweis: Um zu den Modulen zu gelangen"
  //: /line:"kann man auch Rechtsklicken und <font color=red2>Öffnen</font>"
  //: /line:"aus dem Popup Menu aufrufen."
  //: /end
  //: LED g5 (w0) @(277,326) /sn:0 /R:2 /w:[ 0 ] /type:0
  ADD4 g0 (.Cin(w1), .A(w5), .B(w6), .Cout(w0), .S(w4));   //: @(245, 221) /sz:(64, 64) /sn:0 /p:[ Ti0>0 Li0>1 Li1>1 Bo0<1 Ro0<0 ]
  //: comment g12 @(10,410) /sn:0 /R:14 /anc:1
  //: /line:"<tutorial-navigation byfile=1>"
  //: /end

endmodule
//: /netlistEnd

//: /netlistBegin ADD4
module ADD4(A, Cout, Cin, B, S);
//: interface  /sz:(64, 64) /bd:[ Ti0>Cin(34/64) Li0>A[3:0](16/64) Li1>B[3:0](41/64) Bo0<Cout(32/64) Ro0<S[3:0](27/64) ]
input [3:0] B;    //: /sn:0 {0}(#:35,285)(#:49,285){1}
input [3:0] A;    //: /sn:0 {0}(#:48,156)(#:79,156){1}
output Cout;    //: /sn:0 {0}(358,477)(358,444){1}
input Cin;    //: /sn:0 {0}(358,98)(358,137){1}
output [3:0] S;    //: /sn:0 {0}(636,288)(#:560,288){1}
wire A0;    //: /sn:0 {0}(85,141)(311,141)(311,150)(326,150){1}
wire w7;    //: /sn:0 {0}(554,293)(406,293)(406,328)(391,328){1}
wire A3;    //: /sn:0 {0}(85,171)(107,171)(107,405)(326,405){1}
wire w3;    //: /sn:0 {0}(358,189)(358,222){1}
wire A6;    //: /sn:0 {0}(55,280)(215,280)(215,257)(326,257){1}
wire A2;    //: /sn:0 {0}(85,161)(116,161)(116,319)(326,319){1}
wire B2;    //: /sn:0 {0}(55,290)(211,290)(211,341)(326,341){1}
wire w1;    //: /sn:0 {0}(554,273)(515,273)(515,159)(391,159){1}
wire w8;    //: /sn:0 {0}(358,392)(358,358){1}
wire w2;    //: /sn:0 {0}(554,283)(406,283)(406,244)(391,244){1}
wire A1;    //: /sn:0 {0}(85,151)(125,151)(125,228)(311,228)(311,235)(326,235){1}
wire w10;    //: /sn:0 {0}(554,303)(512,303)(512,414)(391,414){1}
wire w5;    //: /sn:0 {0}(358,306)(358,274){1}
wire B3;    //: /sn:0 {0}(55,300)(99,300)(99,427)(326,427){1}
wire B0;    //: /sn:0 {0}(55,270)(205,270)(205,172)(326,172){1}
//: enddecls

  //: IN g4 (A) @(46,156) /sn:0 /w:[ 0 ]
  assign S = {w10, w7, w2, w1}; //: CONCAT g8  @(559,288) /sn:0 /w:[ 1 0 0 0 0 ] /dr:1 /tp:0 /drp:1
  ADD1 g3 (.Cin(w8), .A(A3), .B(B3), .Cout(Cout), .S(w10));   //: @(327, 393) /sz:(63, 50) /sn:0 /p:[ Ti0>0 Li0>1 Li1>1 Bo0<1 Ro0<1 ]
  ADD1 g2 (.Cin(w5), .A(A2), .B(B2), .Cout(w8), .S(w7));   //: @(327, 307) /sz:(63, 50) /sn:0 /p:[ Ti0>0 Li0>1 Li1>1 Bo0<1 Ro0<1 ]
  ADD1 g1 (.Cin(w3), .A(A1), .B(A6), .Cout(w5), .S(w2));   //: @(327, 223) /sz:(63, 50) /sn:0 /p:[ Ti0>1 Li0>1 Li1>1 Bo0<1 Ro0<1 ]
  //: OUT g11 (Cout) @(358,474) /sn:0 /R:3 /w:[ 0 ]
  //: IN g10 (Cin) @(358,96) /sn:0 /R:3 /w:[ 0 ]
  //: IN g6 (B) @(33,285) /sn:0 /w:[ 0 ]
  assign {B3, B2, A6, B0} = B; //: CONCAT g7  @(50,285) /sn:0 /R:2 /w:[ 0 0 0 0 1 ] /dr:0 /tp:0 /drp:0
  //: OUT g9 (S) @(633,288) /sn:0 /w:[ 0 ]
  assign {A3, A2, A1, A0} = A; //: CONCAT g5  @(80,156) /sn:0 /R:2 /w:[ 0 0 0 0 1 ] /dr:0 /tp:0 /drp:0
  ADD1 g0 (.Cin(Cin), .A(A0), .B(B0), .Cout(w3), .S(w1));   //: @(327, 138) /sz:(63, 50) /sn:0 /p:[ Ti0>1 Li0>1 Li1>1 Bo0<0 Ro0<1 ]

endmodule
//: /netlistEnd

//: /netlistBegin ADD1
module ADD1(B, Cin, Cout, S, A);
//: interface  /sz:(63, 50) /bd:[ Ti0>Cin(31/63) Li0>A(12/50) Li1>B(34/50) Bo0<Cout(31/63) Ro0<S(21/50) ]
input B;    //: /sn:0 {0}(113,161)(170,161){1}
//: {2}(174,161)(222,161){3}
//: {4}(172,159)(172,98)(217,98){5}
input A;    //: /sn:0 {0}(112,93)(181,93){1}
//: {2}(185,93)(50:217,93){3}
//: {4}(183,95)(183,156)(222,156){5}
output Cout;    //: /sn:0 {0}(411,157)(336,157){1}
input Cin;    //: /sn:0 {0}(274,117)(274,93){1}
//: {2}(276,91)(314,91){3}
//: {4}(274,89)(274,41)(109,41){5}
output S;    //: /sn:0 {0}(408,94)(-39:335,94){1}
wire w6;    //: /sn:0 {0}(315,159)(243,159){1}
wire Cin0;    //: /sn:0 {0}(314,96)(271,96){1}
//: {2}(267,96)(238,96){3}
//: {4}(269,98)(269,117){5}
wire w0;    //: /sn:0 {0}(315,154)(272,154)(272,138){1}
//: enddecls

  _GGXOR2 #(8) g4 (.I0(A), .I1(B), .Z(Cin0));   //: @(228,96) /sn:0 /w:[ 3 5 3 ]
  _GGNAND2 #(4) g8 (.I0(w0), .I1(w6), .Z(Cout));   //: @(326,157) /sn:0 /w:[ 0 0 1 ]
  //: joint g3 (B) @(172, 161) /w:[ 2 4 1 -1 ]
  //: joint g13 (Cin) @(274, 91) /w:[ 2 4 -1 1 ]
  _GGNAND2 #(4) g2 (.I0(A), .I1(B), .Z(w6));   //: @(233,159) /sn:0 /w:[ 5 3 1 ]
  //: IN g1 (B) @(111,161) /sn:0 /w:[ 0 ]
  //: joint g11 (Cin0) @(269, 96) /w:[ 1 -1 2 4 ]
  //: IN g10 (Cin) @(107,41) /sn:0 /w:[ 5 ]
  //: OUT g6 (Cout) @(408,157) /sn:0 /w:[ 0 ]
  _GGXOR2 #(8) g7 (.I0(Cin), .I1(Cin0), .Z(S));   //: @(325,94) /sn:0 /w:[ 3 0 1 ]
  _GGNAND2 #(4) g9 (.I0(Cin0), .I1(Cin), .Z(w0));   //: @(272,128) /sn:0 /R:3 /w:[ 5 0 1 ]
  //: OUT g5 (S) @(405,94) /sn:0 /w:[ 0 ]
  //: joint g0 (A) @(183, 93) /w:[ 2 -1 1 4 ]
  //: IN g12 (A) @(110,93) /sn:0 /w:[ 0 ]

endmodule
//: /netlistEnd


`timescale 1ns/1ns


//: /builtinBegin
module _GGXOR2 #(.Diz(1)) (I0, I1,  Z);
   output  Z;
   input   I0, I1;

     assign #Diz Z = ( I0 ^ I1 );
   
endmodule // xor

//: /builtinEnd


//: /builtinBegin
module _GGNAND2 #(.Diz(1)) (I0, I1,  Z);
   output  Z;
   input   I0, I1;
   reg  Z;

   assign #Diz Z = ~( I0 & I1 );
   
endmodule // and
//: /builtinEnd

