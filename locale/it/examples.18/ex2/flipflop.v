//: version "1.8.3"
//: property useExtBars = 0

module main;    //: root_module
supply0 w4;    //: /sn:0 {0}(661,215)(661,212)(640,212)(640,229){1}
supply0 w3;    //: /sn:0 {0}(499,211)(499,207)(483,207)(483,226){1}
supply0 w0;    //: /sn:0 {0}(346,226)(346,200)(362,200)(362,210){1}
wire w7;    //: /sn:0 {0}(494,247)(504,247){1}
wire ww;    //: /sn:0 /dp:9 {0}(336,226)(336,171){1}
//: {2}(338,169)(471,169){3}
//: {4}(475,169)(630,169)(630,229){5}
//: {6}(473,171)(473,226){7}
//: {8}(334,169)(178,169){9}
wire w16;    //: /sn:0 {0}(435,235)(446,235)(446,242)(462,242){1}
wire w19;    //: /sn:0 {0}(555,270)(564,270)(564,247)(574,247){1}
wire w15;    //: /sn:0 {0}(458,122)(458,139)(380,139)(380,235){1}
//: {2}(382,237)(414,237){3}
//: {4}(378,237)(357,237){5}
//: {6}(380,239)(380,272)(534,272){7}
wire zz;    //: /sn:0 /dp:1 {0}(175,344)(339,344){1}
//: {2}(343,344)(476,344){3}
//: {4}(480,344)(635,344)(635,261){5}
//: {6}(478,342)(478,258){7}
//: {8}(341,342)(341,258){9}
wire w1;    //: /sn:0 {0}(357,247)(367,247)(367,277)(312,277)(312,242)(325,242){1}
wire [2:0] w17;    //: /sn:0 {0}(448,116)(448,91){1}
wire w12;    //: /sn:0 {0}(595,245)(619,245){1}
wire w11;    //: /sn:0 {0}(438,122)(438,152)(677,152)(677,188){1}
//: {2}(675,190)(564,190)(564,242)(574,242){3}
//: {4}(677,192)(677,240)(651,240){5}
wire w13;    //: /sn:0 {0}(651,250)(661,250){1}
wire w5;    //: /sn:0 {0}(494,237)(518,237){1}
//: {2}(520,235)(520,194){3}
//: {4}(520,190)(520,133)(448,133)(448,122){5}
//: {6}(518,192)(404,192)(404,232)(414,232){7}
//: {8}(520,239)(520,267)(534,267){9}
//: enddecls

  //: supply0 g8 (w3) @(499,217) /sn:0 /w:[ 0 ]
  clock g4 (.Z(zz));   //: @(162,344) /sn:0 /w:[ 0 ] /omega:100 /phi:0 /duty:50
  //: supply0 g3 (w0) @(362,216) /sn:0 /w:[ 1 ]
  led g13 (.I(w17));   //: @(448,84) /sn:0 /w:[ 1 ] /type:1
  ff g2 (.Q(w11), ._Q(w13), .D(w12), .EN(w4), .CLR(ww), .CK(zz));   //: @(635,245) /sn:0 /w:[ 5 0 1 1 5 5 ]
  ff g1 (.Q(w5), ._Q(w7), .D(w16), .EN(w3), .CLR(ww), .CK(zz));   //: @(478,242) /sn:0 /w:[ 0 0 1 1 7 7 ]
  //: joint g16 (w5) @(520, 192) /w:[ -1 4 6 3 ]
  //: joint g11 (zz) @(341, 344) /w:[ 2 8 1 -1 ]
  //: comment g10 /dolink:0 /link:"" @(103,373) /sn:0
  //: /line:"Simulazione di un circuito sequenziale"
  //: /line:""
  //: /line:"1) Clicca con il tasto destro sul collegamento del clock e scegli \"Properties\"."
  //: /line:"   Nel formulario che appare inserisci il nome \"ck\" e disattiva il pulsante \"Hide Name\"."
  //: /line:"   \"OK\""
  //: /line:"2) Fai lo stesso sul collegamento dell'interruttore, inserisci il nome \"_clr\" e "
  //: /line:"   disattiva \"Hide Name\". \"OK\""
  //: /line:"   Nota come il carattere \"_\" che precede il nome di un segnale viene rappresentato come "
  //: /line:"   sopralineatura (segnale attivo basso)."
  //: /line:"3) Attiva la simulazione e aggancia le sonde ai segnali \"ck\", \"clr\" e a quelli che vanno"
  //: /line:"   al visualizzatore. "
  //: /line:"   Clicca su \"Play per entrare in modalità  di simulazione continua."
  //: /line:"   Il visualizzatore di timing traccia i segnali senza fermarsi."
  //: /line:"4) Ritorna in modo \"ciclo singolo\" cliccando sull'icona \"Pause\" oppure inserendo il "
  //: /line:"   carattere Tab."
  //: /line:"   Inserisci ora qualche carattere Tab e osserva come la simulazione avanza di un ciclo"
  //: /line:"   per ogni Tab."
  //: /line:"   Nota il tempo di \"overstep\": la simulazione continua per un certo tempo oltre "
  //: /line:"   il fianco attivo del clock."
  //: /line:"   "
  //: /end
  and g19 (.I0(w5), .I1(w15), .Z(w19));   //: @(545,270) /sn:0 /w:[ 9 7 0 ]
  //: joint g6 (ww) @(336, 169) /w:[ 2 -1 8 1 ]
  //: supply0 g9 (w4) @(661,221) /sn:0 /w:[ 0 ]
  //: joint g7 (ww) @(473, 169) /w:[ 4 -1 3 6 ]
  //: joint g20 (w5) @(520, 237) /w:[ -1 2 1 8 ]
  xor g15 (.I0(w5), .I1(w15), .Z(w16));   //: @(425,235) /sn:0 /w:[ 7 3 0 ]
  //: joint g17 (w15) @(380, 237) /w:[ 2 1 4 6 ]
  concat g14 (.I0(w15), .I1(w5), .I2(w11), .Z(w17));   //: @(448,117) /sn:0 /R:1 /w:[ 0 5 0 0 ] /dr:0
  //: switch g5 (ww) @(161,169) /sn:0 /w:[ 9 ] /st:1
  //: joint g21 (w11) @(677, 190) /w:[ -1 1 2 4 ]
  ff g0 (.Q(w15), ._Q(w1), .D(w1), .EN(w0), .CLR(ww), .CK(zz));   //: @(341,242) /sn:0 /w:[ 5 0 1 0 0 9 ]
  xor g18 (.I0(w11), .I1(w19), .Z(w12));   //: @(585,245) /sn:0 /w:[ 3 1 0 ]
  //: joint g12 (zz) @(478, 344) /w:[ 4 6 3 -1 ]

endmodule
