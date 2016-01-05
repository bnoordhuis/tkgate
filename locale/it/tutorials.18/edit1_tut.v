//: version "1.8.3"
//: property useExtBars = 0
//: property discardChanges = 1

module main;    //: root_module
wire w7;    //: /sn:0 {0}(682,363)(697,363)(697,375){1}
wire s0;    //: {0}(750,310)(780,310){1}
wire w4;    //: /sn:0 {0}(702,345)(702,375){1}
wire c0;    //: {0}(598,268)(704,268)(704,310){1}
//: {2}(706,312)(729,312){3}
//: {4}(704,314)(704,324){5}
wire c1;    //: {0}(700,396)(700,433){1}
//: {2}(702,435)(725,435){3}
//: {4}(700,437)(700,447){5}
wire a1;    //: {0}(598,427)(638,427){1}
//: {2}(642,427)(650,427){3}
//: {4}(640,429)(640,483)(657,483){5}
wire s1;    //: /dp:1 {0}(746,433)(781,433){1}
wire w8;    //: /sn:0 {0}(678,486)(693,486)(693,498){1}
wire b1;    //: {0}(598,462)(632,462){1}
//: {2}(634,460)(634,432)(650,432){3}
//: {4}(634,464)(634,488)(657,488){5}
wire a0;    //: /dp:1 {0}(598,304)(642,304){1}
//: {2}(646,304)(654,304){3}
//: {4}(644,306)(644,360)(661,360){5}
wire b0;    //: /dp:1 {0}(598,339)(636,339){1}
//: {2}(638,337)(638,309)(654,309){3}
//: {4}(638,341)(638,365)(661,365){5}
wire c2;    //: {0}(783,530)(696,530)(696,519){1}
wire w10;    //: /sn:0 {0}(725,430)(697,430){1}
//: {2}(693,430)(671,430){3}
//: {4}(695,432)(695,447){5}
wire w5;    //: /sn:0 {0}(698,468)(698,498){1}
wire w9;    //: /sn:0 {0}(729,307)(701,307){1}
//: {2}(697,307)(675,307){3}
//: {4}(699,309)(699,324){5}
//: enddecls

  //: comment g61 /dolink:1 /link:"@T/edwire_tut.v" @(630,100) /sn:0 /anc:1
  //: /line:"Collegamenti"
  //: /end
  nand g4 (.I0(w8), .I1(w5), .Z(c2));   //: @(696,509) /sn:0 /R:3 /w:[ 1 1 1 ]
  //: joint g8 (c1) @(700, 435) /w:[ 2 1 -1 4 ]
  //: comment g3 /dolink:0 /link:"" @(28,262) /anc:1
  //: /line:"5) Nel menu \"Tool\" selziona \"Bit Width\": il puntatore diventa "
  //: /line:"un cavo piatto."
  //: /line:"In basso a destra della finestra principale appare un campo di scrittura "
  //: /line:"con la dicitura \"Bits:\".  Clicca sul campo e inserisci un numero"
  //: /line:"compreso fra 1 e 32. Se clicci ora su un collegamento, questo diventa un "
  //: /line:"bus della larghezza indicata."
  //: /line:""
  //: /line:"6) Doppio click su un collegamento apre un formulario con le sue"
  //: /line:"proprietà: puoi cambiare il nome del segnale, renderlo visibile"
  //: /line:"o impostare la larghezza (numero di bit) del collegamento."
  //: /line:"Lo stesso formulario può essere ottenuto dal menu popup che appare"
  //: /line:"cliccando con il tasto destro sul collegamento."
  //: /line:""
  //: /line:"7) Apri il menu \"Tool\" e seleziona \"Move/Connect\" per tornare al "
  //: /line:"cursore di edizione base."
  //: /line:"Ora clicca doppio (o clicca con il tasto destro e seleziona "
  //: /line:"\"Properties...\") su una porta XOR. Nel formulario che appare"
  //: /line:"puoi cambiare il nome della porta, renderlo visibile (cancella"
  //: /line:"l'opzione \"hide\"), aggiungere o eliminare ingressi."
  //: /line:""
  //: /line:"8) Clicca con il tasto destro su una porta e attiva l'opzione"
  //: /line:"\"Anchor\". Ora prova a spostare la porta (non dovrebbe spostarsi). "
  //: /line:"Questa opzione può essere usata per evitare di spostare accidentalmente"
  //: /line:"oggetti che vuoi rimangano in una posizione fissa."
  //: /end
  //: comment g13 /dolink:0 /link:"@T/edit1_tut.v" @(630,70) /sn:0 /anc:1
  //: /line:"Manipolazioni base"
  //: /end
  xor g34 (.I0(w10), .I1(c1), .Z(s1));   //: @(736,433) /sn:0 /w:[ 0 3 0 ]
  led g37 (.I(s0));   //: @(787,310) /sn:0 /R:3 /w:[ 1 ] /type:0
  //: comment g2 /dolink:0 /link:"" @(29,38) /anc:1
  //: /line:"1) Clicca su una porta logica con il tasto sinistro del mouse e trascinala"
  //: /line:"in una nuova posizione. Osserva come i fili si muovono con la porta."
  //: /line:""
  //: /line:"2) Nel menu \"Tool\" seleziona \"Cut Wire\": il puntatore assume la forma"
  //: /line:"di un tronchesino."
  //: /line:"Per tagliare un filo clicca con il tronchesino sullo stesso. Segmenti flottanti"
  //: /line:"o segmanti collegati ad una giunzione verranno eliminati automaticamente."
  //: /line:"Puoi accedere alle opzioni del menu \"Tool\" attraverso il menu popup che"
  //: /line:"ottieni cliccando con il tasto destro del mouse in campo libero."
  //: /line:""
  //: /line:"3) Nel menu \"Tool\" seleziona \"Invert\"."
  //: /line:"Clicca vicino ad un ingresso o un'uscita di una porta per inserire un inverter."
  //: /line:""
  //: /line:"4) Nel menu \"Tool\" seleziona \"Delete Gate\": appare un estrattore di IC."
  //: /line:"Se clicchi su una porta con l'estrattore, questa viene cancellata. "
  //: /line:"La cancellazione può avvenire anche selezionando la/le porte"
  //: /line:"da cancellare e usando il tasto \"Delete\" o cliccando con il tasto destro"
  //: /line:"e selezionando \"Delete\"."
  //: /end
  nand g1 (.I0(w7), .I1(w4), .Z(c1));   //: @(700,386) /sn:0 /R:3 /w:[ 1 1 0 ]
  //: comment g16 /dolink:1 /link:"@T/module_tut.v" @(630,115) /sn:0 /anc:1
  //: /line:"Moduli"
  //: /end
  //: switch g11 (a1) @(581,427) /sn:0 /w:[ 0 ] /st:0
  nand g28 (.I0(a0), .I1(b0), .Z(w7));   //: @(672,363) /sn:0 /w:[ 5 5 0 ]
  //: joint g10 (a1) @(640, 427) /w:[ 2 -1 1 4 ]
  //: joint g32 (w9) @(699, 307) /w:[ 1 -1 2 4 ]
  //: switch g27 (c0) @(581,268) /sn:0 /w:[ 0 ] /st:0
  led g19 (.I(s1));   //: @(788,433) /sn:0 /R:3 /w:[ 1 ] /type:0
  nand g6 (.I0(a1), .I1(b1), .Z(w8));   //: @(668,486) /sn:0 /w:[ 5 5 0 ]
  //: joint g38 (b1) @(634, 462) /w:[ -1 2 1 4 ]
  //: joint g7 (w10) @(695, 430) /w:[ 1 -1 2 4 ]
  nand g9 (.I0(w10), .I1(c1), .Z(w5));   //: @(698,458) /sn:0 /R:3 /w:[ 5 5 0 ]
  nand g31 (.I0(w9), .I1(c0), .Z(w4));   //: @(702,335) /sn:0 /R:3 /w:[ 5 5 0 ]
  //: comment g20 /dolink:1 /link:"@T/create_tut.v" @(630,55) /sn:0 /anc:1
  //: /line:"Creare un circuito"
  //: /end
  //: frame g15 @(600,25) /sn:0 /anc:1 /wi:213 /ht:149 /tx:"Tutorials"
  led g39 (.I(c2));   //: @(790,530) /sn:0 /R:3 /w:[ 0 ] /type:0
  //: joint g29 (a0) @(644, 304) /w:[ 2 -1 1 4 ]
  //: switch g25 (a0) @(581,304) /sn:0 /w:[ 0 ] /st:0
  //: comment g17 /dolink:1 /link:"@T/sim_tut.v" @(630,130) /sn:0 /anc:1
  //: /line:"Simulazione combinatoria"
  //: /end
  //: frame g5 @(550,216) /sn:0 /wi:266 /ht:355 /tx:"Modifica questo circuito"
  //: comment g14 /dolink:1 /link:"@T/seqsim_tut.v" @(630,145) /sn:0 /anc:1
  //: /line:"Simulazione sequenziale"
  //: /end
  xor g24 (.I0(w9), .I1(c0), .Z(s0));   //: @(740,310) /sn:0 /w:[ 0 3 0 ]
  //: comment g21 /dolink:1 /link:"@T/edit2_tut.v" @(630,85) /sn:0 /anc:1
  //: /line:"Gruppi di oggetti"
  //: /end
  //: switch g36 (b1) @(581,462) /sn:0 /w:[ 0 ] /st:0
  xor g23 (.I0(a0), .I1(b0), .Z(w9));   //: @(665,307) /sn:0 /w:[ 3 3 3 ]
  //: switch g26 (b0) @(581,339) /sn:0 /w:[ 0 ] /st:0
  //: comment g0 /dolink:0 /link:"" @(8,10) /anc:1
  //: /line:"Questo tutorial introduce alle funzioni base per l'edizione di circuiti con TkGate."
  //: /end
  //: comment g22 /dolink:0 /link:"" @(605,70) /sn:0 /anc:1
  //: /line:"->"
  //: /end
  xor g35 (.I0(a1), .I1(b1), .Z(w10));   //: @(661,430) /sn:0 /w:[ 3 3 3 ]
  //: comment g18 /dolink:1 /link:"@T/welcome_tut.v" @(630,40) /sn:0 /anc:1
  //: /line:"Introduzione a TkGate"
  //: /end
  //: joint g33 (c0) @(704, 312) /w:[ 2 1 -1 4 ]
  //: joint g30 (b0) @(638, 339) /w:[ -1 2 1 4 ]

endmodule
