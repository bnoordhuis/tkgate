//: version "1.8.3"
//: property useExtBars = 0
//: property discardChanges = 1

module main;    //: root_module
wire w3;    //: /sn:0 {0}(744,264)(679,264){1}
wire w1;    //: /sn:0 {0}(744,259)(709,259)(709,224)(679,224){1}
wire w2;    //: /sn:0 {0}(685,302)(679,302){1}
wire w5;    //: /sn:0 {0}(765,261)(775,261){1}
//: enddecls

  //: comment g61 /dolink:1 /link:"@T/edwire_tut.v" @(630,100) /sn:0 /anc:1
  //: /line:"Collegamenti"
  //: /end
  and g4 (.I0(w1), .I1(w3), .Z(w5));   //: @(755,261) /sn:0 /w:[ 0 0 0 ]
  //: switch g3 (w2) @(662,302) /sn:0 /w:[ 1 ] /st:0
  //: switch g2 (w3) @(662,264) /sn:0 /w:[ 1 ] /st:0
  //: switch g1 (w1) @(662,224) /sn:0 /w:[ 1 ] /st:0
  //: comment g16 /dolink:1 /link:"@T/module_tut.v" @(630,115) /sn:0 /anc:1
  //: /line:"Moduli"
  //: /end
  //: frame g11 @(434,347) /sn:0 /anc:1 /wi:373 /ht:230 /tx:"Copia qui"
  //: frame g10 @(22,347) /sn:0 /anc:1 /wi:394 /ht:227 /tx:"Trascina qui"
  //: frame g15 @(600,25) /sn:0 /anc:1 /wi:213 /ht:149 /tx:"Tutorials"
  //: comment g20 /dolink:1 /link:"@T/create_tut.v" @(630,55) /sn:0 /anc:1
  //: /line:"Creare un circuito"
  //: /end
  //: comment g17 /dolink:1 /link:"@T/sim_tut.v" @(630,130) /sn:0 /anc:1
  //: /line:"Simulazione combinatoria"
  //: /end
  //: comment g14 /dolink:1 /link:"@T/seqsim_tut.v" @(630,145) /sn:0 /anc:1
  //: /line:"Simulazione sequenziale"
  //: /end
  //: comment g5 /dolink:0 /link:"" @(38,36) /anc:1
  //: /line:"1) Premi il tasto sinistro del mouse a sinistra sopra i tre interruttori"
  //: /line:"e la porta AND. Tieni premuto il tasto e trascina verso il basso a destra"
  //: /line:"in modo che il rettangolo di selezione che appare circondi tutto "
  //: /line:"il circuito. Rilascia il tasto. Gli interruttori e la porta logica risultano"
  //: /line:"evidenziati per indicare che sono selezionati."
  //: /line:""
  //: /line:"2) Premi il tasto sinistro del mouse sopra una delle porte selezionate e"
  //: /line:"trascina. Tutto il gruppo selezionato segue: spostalo fino nel riquadro"
  //: /line:"etichettato con \"Trascina qui\". Clicca in campo libero (dove non ci sono né"
  //: /line:"porte né collegamenti) per disattivare la selezione."
  //: /line:""
  //: /line:"3) Seleziona la AND, entra nel menu \"Gate\" e seleziona \"Add Input\".  "
  //: /line:"Alla porta viene aggiunto un ingresso che ora puoi collegare al terzo interruttore."
  //: /line:""
  //: /line:"4) Segui di nuovo le istruzioni del punto 1 per selezionare il circuito."
  //: /line:"Nel menu \"Edit\" seleziona \"Copy\": il gruppo di oggetti selezionati viene copiato "
  //: /line:"nel buffer di taglio."
  //: /line:"Clicca ora con il tasto sinistro al centro del riquadro etichettato \"Copia qui\" e "
  //: /line:"seleziona \"Paste\" dal menu \"Edit\". Verrà  creata una copia delle porte selezionate."
  //: /line:"È possibile eliminare un gruppo di oggetti selezionati con il tasto \"Delete\"."
  //: /line:""
  //: /line:"5) Puoi creare un gruppo di oggetti cliccando su ogni oggetto che vuoi nel gruppo"
  //: /line:"mentre mantieni pigiato il tasto <control>."
  //: /line:""
  //: /end
  //: comment g21 /dolink:0 /link:"@T/edit2_tut.v" @(630,85) /sn:0 /anc:1
  //: /line:"Gruppi di oggetti"
  //: /end
  //: comment g22 /dolink:0 /link:"" @(605,85) /sn:0 /anc:1
  //: /line:"->"
  //: /end
  //: comment g0 /dolink:0 /link:"" @(26,17) /anc:1
  //: /line:"Questo tutorial introduce alle operazioni sui gruppi di oggetti."
  //: /end
  //: comment g18 /dolink:1 /link:"@T/welcome_tut.v" @(630,40) /sn:0 /anc:1
  //: /line:"Introduzione a TkGate"
  //: /end
  //: comment g12 /dolink:1 /link:"@T/edit1_tut.v" @(630,70) /sn:0 /anc:1
  //: /line:"Manipolazioni base"
  //: /end

endmodule
