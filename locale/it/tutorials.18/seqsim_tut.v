//: version "1.8.3"
//: property useExtBars = 0
//: property discardChanges = 1

module main;    //: root_module
supply0 w0;    //: /sn:0 /dp:1 {0}(707,320)(730,320)(730,335){1}
supply0 w3;    //: /sn:0 {0}(705,275)(705,265)(692,265){1}
wire [7:0] w7;    //: /sn:0 {0}(652,225)(652,251){1}
wire w4;    //: /sn:0 /dp:1 {0}(707,310)(718,310)(718,298)(728,298){1}
wire [7:0] w2;    //: /sn:0 /dp:1 {0}(668,305)(668,280){1}
wire ck;    //: /dp:1 {0}(631,315)(566,315){1}
wire [7:0] reg_out;    //: {0}(668,326)(668,368)(724,368){1}
//: {2}(728,368)(784,368)(784,241)(684,241)(684,251){3}
//: {4}(726,370)(726,400){5}
wire w9;    //: /sn:0 {0}(644,265)(634,265){1}
//: enddecls

  //: comment g61 /dolink:1 /link:"@T/edwire_tut.v" @(630,100) /sn:0 /anc:1
  //: /line:"Collegamenti"
  //: /end
  //: supply0 g4 (w0) @(730,341) /sn:0 /w:[ 1 ]
  //: joint g8 (reg_out) @(726, 368) /w:[ 2 -1 1 4 ]
  //: comment g13 /dolink:0 /link:"" @(24,45) /anc:1
  //: /line:"1) Clicca sull'icona del \"Logic Analyzer\" nella barra degli strumenti"
  //: /line:"o seleziona \"Begin Simulation\" dal menu \"Simulate\".  Dovrebbe apparire"
  //: /line:"la finestra dei timing."
  //: /line:""
  //: /line:"2) Aggancia una sonda alla linea del clock (indicata con il nome \"ck\")"
  //: /line:"e sull'uscita del registro (\"reg_out\") cliccando doppio sul rispettivo"
  //: /line:"collegamento. Nella finestra dei timing appare una traccia per ogni "
  //: /line:"segnale."
  //: /line:""
  //: /line:"3) Inserisci spazi (tastiera) per far avanzare il tempo di simulazione."
  //: /line:"Ogni spazio fa avanzare il tempo di simulazione di un multiplo di epoche."
  //: /line:"Una \"epoca\" corrisponde alla risoluzione del simulatore (l'unità  di tempo"
  //: /line:"minima elaborata)."
  //: /line:"Vedi il tutorial sulla simulazione combinatoria per informazioni sulla "
  //: /line:"possibilità  di impostare il passo di simulazione (multipli di epoche)."
  //: /line:""
  //: /line:"4) Puoi far avanzare la simulazione di un ciclo di clock inserendo "
  //: /line:"il carattere Tab o selezionando \"Step Cycle\" dal menu \"Simulate\". "
  //: /line:"La simulazione avanzerà  fino al primo fianco positivo di uno "
  //: /line:"qualsiasi dei segnali di clock del circuito (il primo che si verifica), "
  //: /line:"più un certo numero di epoche, dette di \"overstep\". "
  //: /line:"Il tempo di overstep è previsto per dare ai registri il "
  //: /line:"tempo per acquisire i nuovi valori prima di fermare"
  //: /line:"la simulazione. Puoi cambiare la quantità  di overstep, "
  //: /line:"il numero di cicli di avanzamento, o selezionare un solo"
  //: /line:"segnale di clock per il passo di simulazione usando il formulario "
  //: /line:"di opzioni (File - Options - ...)."
  //: /line:""
  //: /line:"5) In questo esempio, imposta l'interruttore dell'entrata CLR su \"off\""
  //: /line:"e fai avanzare la simulazione di un ciclo per azzerare il registro."
  //: /line:"Poi commuta su \"on\" e avanza alcuni cicli di clock per far avanzare il "
  //: /line:"contatore."
  //: /line:""
  //: /line:"6) Clicca sull'interruttore a 8 vie (\"dip switch\"): si apre un "
  //: /line:"formulario che permette di inserire il valore dato dalla posizione"
  //: /line:"dei singoli contatti. Inserisci un valore in base esadecimale e "
  //: /line:"clicca \"Apply\" per cambiare il passo di conteggio."
  //: /line:""
  //: /line:"7) Se clicchi e tieni pigiato il tasto su un collegamento qualsiasi, "
  //: /line:"potrai vedere il valore trasportato dal segnale."
  //: /line:""
  //: /line:"8) Clicca sull'icona \"Play\" o seleziona \"Run\" dal menu \"Simulate\". "
  //: /line:"Attivi così la modalità  di simulazione continua."
  //: /line:"Con circuiti sequenziali il simulatore continua a calcolare finchè "
  //: /line:"non premi il pulsante \"Pause\". "
  //: /line:"Puoi cambiare il valore degli interruttori mentre la simulazione "
  //: /line:"è in atto."
  //: /end
  //: switch g3 (w4) @(746,298) /sn:0 /R:2 /w:[ 1 ] /st:1
  register g2 (.Q(reg_out), .D(w2), .EN(w0), .CLR(w4), .CK(ck));   //: @(668,315) /sn:0 /w:[ 0 0 0 0 0 ]
  //: comment g1 /dolink:1 /link:"@T/edit1_tut.v" @(630,70) /sn:0 /anc:1
  //: /line:"Manipolazioni base"
  //: /end
  //: comment g16 /dolink:1 /link:"@T/module_tut.v" @(630,115) /sn:0 /anc:1
  //: /line:"Moduli"
  //: /end
  //: comment g10 /dolink:0 /link:"" @(528,451) /sn:0 /R:2 /anc:1
  //: /line:"9) Puoi spostarti nelle tracce di simulazione"
  //: /line:"usando la barra di scrolling o cliccando"
  //: /line:"sulle tracce e trascinando l'immagine."
  //: /line:"Se clicchi tenendo pigiato il tasto <control>"
  //: /line:"o <shift> otterrai l'azione di zoom in, "
  //: /line:"rispettivamente zoom out."
  //: /line:"Se trascini la barra di scrolling all'estremità "
  //: /line:"destra, la finestra mostrerà  continuamente "
  //: /line:"gli ultimi valori calcolati."
  //: /line:""
  //: /end
  //: dip g6 (w7) @(652,215) /sn:0 /w:[ 0 ] /st:1
  led g7 (.I(reg_out));   //: @(726,407) /sn:0 /R:2 /w:[ 5 ] /type:2
  //: supply0 g9 (w3) @(705,281) /sn:0 /w:[ 0 ]
  //: frame g15 @(600,25) /sn:0 /anc:1 /wi:213 /ht:149 /tx:"Tutorials"
  //: comment g20 /dolink:1 /link:"@T/create_tut.v" @(630,55) /sn:0 /anc:1
  //: /line:"Creare un circuito"
  //: /end
  //: comment g17 /dolink:1 /link:"@T/sim_tut.v" @(630,130) /sn:0 /anc:1
  //: /line:"Simulazione combinatoria"
  //: /end
  //: comment g14 /dolink:0 /link:"@T/seqsim_tut.v" @(630,145) /sn:0 /anc:1
  //: /line:"Simulazione sequenziale"
  //: /end
  add g5 (.A(w7), .B(reg_out), .S(w2), .CI(w3), .CO(w9));   //: @(668,267) /sn:0 /w:[ 1 3 1 1 0 ]
  //: comment g21 /dolink:1 /link:"@T/edit2_tut.v" @(630,85) /sn:0 /anc:1
  //: /line:"Gruppi di oggetti"
  //: /end
  //: comment g22 /dolink:0 /link:"" @(605,145) /sn:0 /anc:1
  //: /line:"->"
  //: /end
  clock g0 (.Z(ck));   //: @(553,315) /sn:0 /w:[ 1 ] /omega:100 /phi:0 /duty:50
  //: comment g18 /dolink:1 /link:"@T/welcome_tut.v" @(630,40) /sn:0 /anc:1
  //: /line:"Introduzione a TkGate"
  //: /end
  //: comment g12 /dolink:0 /link:"" @(16,18) /anc:1
  //: /line:"Questo tutorial introduce alla simulazione di circuiti sequenziali in TkGate."
  //: /line:""
  //: /end

endmodule
