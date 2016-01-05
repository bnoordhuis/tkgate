//: version "1.8.3"
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

  //: comment g61 /dolink:1 /link:"@T/edwire_tut.v" @(630,100) /sn:0 /anc:1
  //: /line:"Collegamenti"
  //: /end
  //: comment g8 /dolink:0 /link:"" @(250,481) /anc:1
  //: /line:"OR gate"
  //: /end
  or g4 (.I0(w5), .I1(w6), .Z(w7));   //: @(243,500) /sn:0 /anc:1 /w:[ 0 0 0 ]
  and g3 (.I0(w2), .I1(w6), .Z(w5));   //: @(198,464) /sn:0 /anc:1 /w:[ 0 3 1 ]
  //: frame g13 @(428,388) /sn:0 /wi:381 /ht:177 /tx:"Disegna qui"
  //: switch g2 (w6) @(124,502) /sn:0 /anc:1 /w:[ 5 ] /st:0
  //: switch g1 (w2) @(124,461) /sn:0 /anc:1 /w:[ 1 ] /st:0
  //: comment g16 /dolink:1 /link:"@T/module_tut.v" @(630,115) /sn:0 /anc:1
  //: /line:"Moduli"
  //: /end
  //: comment g10 /dolink:0 /link:"" @(12,249) /anc:1
  //: /line:"5) Inserisci i due interruttori con la stessa procedura usata per le porte: "
  //: /line:"   a) scegli la posizione;"
  //: /line:"   b) seleziona \"Switch\" dal sottomenu \"I/O\" di \"Make\", oppure digita \"s\"."
  //: /line:""
  //: /line:"6) Ripeti la procedura del punto 4 e collega le uscite degli interruttori con la porta AND, rispettivamente OR."
  //: /line:""
  //: /line:"7) Sull'ingresso inferiore della porta AND clicca e mantieni premuto il tasto sinistro del mouse"
  //: /line:"in modo da far apparire il saldatore. Trascinalo a metà  del collegamento fra interruttore inferiore e porta OR."
  //: /line:"Rilascia il tasto."
  //: /line:"Un punto di giunzione viene inserito automaticamente."
  //: /end
  //: comment g6 /dolink:0 /link:"" @(101,427) /anc:1
  //: /line:"Switches"
  //: /end
  //: comment g9 /dolink:0 /link:"" @(12,9) /anc:1
  //: /line:"1) In questo esempio realizzerai un circuito come quello mostrato sotto. "
  //: /line:"Disegna il circuito nell'area incorniciata in basso a destra. "
  //: /line:"Per cominciare, definisci la posizione per la porta AND cliccando il tasto "
  //: /line:"destro del mouse nella posizione voluta. Apparirà  una \"X\" per indicare"
  //: /line:"il punto di inserimento."
  //: /line:""
  //: /line:"2) Per inserire una porta AND nella posizione scelta, seleziona \"AND\""
  //: /line:"dal sottomenu \"Gate\" del popup \"Make\" (o da \"Make\" della barra dei menu)."
  //: /line:"In alternativa puoi inserire il carattere \"a\" da tastiera."
  //: /line:""
  //: /line:"3) Inserisci la porta OR sulla destra, un po' sotto la AND: "
  //: /line:"   a) segna la posizione (\"X\");"
  //: /line:"   b) inserisci la OR scegliendola dal sottomenu \"Gate\" del popup \"Make\" oppure"
  //: /line:"      digita il carattere \"o\"."
  //: /line:""
  //: /line:"4) Collega l'ingresso superiore della porta OR con l'uscita della AND:"
  //: /line:"   a) clicca sull'uscita della porta AND e mantieni premuto il tasto sinistro "
  //: /line:"      del mouse. Il cursore cambia aspetto e diventa un saldatore;"
  //: /line:"   b) trascina il saldatore vicino al filo d'ingresso della porta OR e rilascia"
  //: /line:"      il tasto. I fili si agganciano."
  //: /end
  //: comment g7 /dolink:0 /link:"" @(193,441) /anc:1
  //: /line:"AND gate"
  //: /end
  //: frame g15 @(600,25) /sn:0 /anc:1 /wi:213 /ht:149 /tx:"Tutorials"
  //: comment g20 /dolink:0 /link:"@T/create_tut.v" @(630,55) /sn:0 /anc:1
  //: /line:"Creare un circuito"
  //: /end
  //: comment g17 /dolink:1 /link:"@T/sim_tut.v" @(630,130) /sn:0 /anc:1
  //: /line:"Simulazione combinatoria"
  //: /end
  //: comment g14 /dolink:1 /link:"@T/seqsim_tut.v" @(630,145) /sn:0 /anc:1
  //: /line:"Simulazione sequenziale"
  //: /end
  //: joint g5 (w6) @(175, 502) /anc:1 /w:[ 1 2 4 -1 ]
  //: comment g21 /dolink:1 /link:"@T/edit2_tut.v" @(630,85) /sn:0 /anc:1
  //: /line:"Gruppi di oggetti"
  //: /end
  //: comment g22 /dolink:0 /link:"" @(605,55) /sn:0 /anc:1
  //: /line:"->"
  //: /end
  //: comment g0 /dolink:1 /link:"@T/edit1_tut.v" @(630,70) /sn:0 /anc:1
  //: /line:"Manipolazioni base"
  //: /end
  //: comment g18 /dolink:1 /link:"@T/welcome_tut.v" @(630,40) /sn:0 /anc:1
  //: /line:"Introduzione a TkGate"
  //: /end

endmodule
