//: version "1.8.3"
//: property useExtBars = 0
//: property discardChanges = 1

module foo();
//: interface  /sz:(40, 40) /bd:[ ]
//: enddecls

  //: comment g61 /dolink:1 /link:"@T/edwire_tut.v" @(630,100) /sn:0
  //: /line:"Collegamenti"
  //: /end
  //: comment g3 /dolink:1 /link:"@T/edit1_tut.v" @(630,70) /sn:0
  //: /line:"Manipolazioni base"
  //: /end
  //: frame g1 @(409,199) /sn:0 /wi:404 /ht:316 /tx:"Inserisci i riferimenti alle porte qui"
  //: comment g16 /dolink:0 /link:"@T/module_tut.v" @(630,115) /sn:0
  //: /line:"Moduli"
  //: /end
  //: frame g15 @(600,25) /sn:0 /wi:213 /ht:149 /tx:"Tutorials"
  //: comment g20 /dolink:1 /link:"@T/create_tut.v" @(630,55) /sn:0
  //: /line:"Creare un circuito"
  //: /end
  //: comment g17 /dolink:1 /link:"@T/sim_tut.v" @(630,130) /sn:0
  //: /line:"Simulazione combinatoria"
  //: /end
  //: comment g14 /dolink:1 /link:"@T/seqsim_tut.v" @(630,145) /sn:0
  //: /line:"Simulazione sequenziale"
  //: /end
  //: comment g21 /dolink:1 /link:"@T/edit2_tut.v" @(630,85) /sn:0
  //: /line:"Gruppi di oggetti"
  //: /end
  //: comment g22 /dolink:0 /link:"" @(605,115) /sn:0
  //: /line:"->"
  //: /end
  //: comment g0 /dolink:0 /link:"" @(26,29) /anc:1
  //: /line:"10) Questa è la definizione del modulo \"foo\"."
  //: /line:"Nell'elenco in basso a sinistra della finestra principale ritrovi i nomi "
  //: /line:"delle porte che hai definito nell'istanza del modulo."
  //: /line:"Per creare riferimenti alle porte dei moduli all'interno degli stessi, "
  //: /line:"clicca per posizionare un punto di riferimento nell'area di disegno e "
  //: /line:"seleziona \"Module Input\", \"Module Output\" o \"Module InOut\" dal sottomenu "
  //: /line:"\"Module\" in \"Make\"."
  //: /line:"Appare un formulario dove è possibile inserire il nome della porta."
  //: /line:"Inserisci uno dei nomi delle porte del modulo e seleziona \"OK\"."
  //: /line:""
  //: /line:"11) Puoi anche creare la definizione di un nuovo modulo"
  //: /line:"senza crearne un'istanza se selezioni \"New...\" "
  //: /line:"nel menu \"Module\". "
  //: /line:"Apparirà  un formulario dove potrai inserire il nome "
  //: /line:"del nuovo modulo. Prova a inserire \"bar\" "
  //: /line:"e seleziona \"OK\" (tasto \"return\")."
  //: /line:""
  //: /line:"12) Puoi cambiare il nome, copiare e cancellare"
  //: /line:"moduli usando gli altri comandi del menu \"Module\"."
  //: /line:""
  //: /line:"13) Se hai aperto questo modulo con un doppio click"
  //: /line:"nell'elenco dei moduli, puoi tornare nel modulo "
  //: /line:"principale cliccando doppio ora su \"main\"."
  //: /line:""
  //: /line:"Se hai aperto \"foo\" con il comando \"Open\", puoi "
  //: /line:"tornare al livello superiore selezionando \"Close\""
  //: /line:"nel menu \"Module\" o cliccando doppio su \"main\"."
  //: /end
  //: comment g18 /dolink:1 /link:"@T/welcome_tut.v" @(630,40) /sn:0
  //: /line:"Introduzione a TkGate"
  //: /end

endmodule

module main;    //: root_module
//: enddecls

  //: comment g61 /dolink:1 /link:"@T/edwire_tut.v" @(630,100) /sn:0 /anc:1
  //: /line:"Collegamenti"
  //: /end
  //: comment g3 /dolink:1 /link:"@T/edit1_tut.v" @(630,70) /sn:0 /anc:1
  //: /line:"Manipolazioni base"
  //: /end
  //: frame g2 @(477,189) /sn:0 /wi:330 /ht:350 /tx:"Spazio per creare un'istanza del modulo"
  //: comment g1 /dolink:0 /link:"" @(11,34) /anc:1
  //: /line:"1) Clicca il tasto sinistro al centro dell'area riservata per il disegno"
  //: /line:"in questo tutorial."
  //: /line:"Seleziona \"Module Instance\" dal sottomenu \"Module\" di \"Make\": appare un blocco"
  //: /line:"(sarà  un'istanza del nostro modulo) e si apre un formulario con la rubrica"
  //: /line:"\"Function\" attivata."
  //: /line:"Inserisci il nome \"foo\" in minuscolo e seleziona \"OK\"."
  //: /line:""
  //: /line:"2) Premi il tasto sinistro del mouse sull'angolo in basso a destra dell'istanza"
  //: /line:"e trascina verso il basso e a destra per ottenere un box più grande."
  //: /line:""
  //: /end
  //: comment g16 /dolink:0 /link:"@T/module_tut.v" @(630,115) /sn:0 /anc:1
  //: /line:"Moduli"
  //: /end
  //: comment g6 /dolink:0 /link:"" @(10,160)
  //: /line:"3) Clicca con il tasto destro sul bordo sinistro del blocco e "
  //: /line:"seleziona \"Add Input\" dal popup. Appare un formulario con la "
  //: /line:"rubrica \"Port Name\" attiva. Inserisci \"A\" e seleziona \"OK\"."
  //: /line:"Ripeti la procedura e aggiungi gli ingressi \"B\" and \"C\" dallo "
  //: /line:"stesso lato."
  //: /line:"Ripeti tutto selezionando ora \"Add Output\" per delle porte con"
  //: /line:"i nomi \"X\", \"Y\" e \"Z\" sul lato destro del blocco."
  //: /line:""
  //: /line:"4) Clicca su una delle porte che hai aggiunto: la puoi spostare"
  //: /line:"lungo il lato a cui è agganciata; puoi aggiustarne la"
  //: /line:"posizione."
  //: /line:""
  //: /line:"5) Clicca con il tasto destro vicino ad una delle porte "
  //: /line:"e seleziona \"Change Type\" dal popup. Se ripeti questa "
  //: /line:"operazione la direzione della porta cambia in modo ciclico "
  //: /line:"fra i tre tipi di porta previsti: input, output e inout. "
  //: /line:""
  //: /line:"6) Seleziona \"Cut Wire\" dal menu \"Tool\" e clicca sulla porta \"Z\""
  //: /line:"in prossimità  del bordo del blocco. La porta viene cancellata."
  //: /line:""
  //: /line:"7) Seleziona \"Move/Connect\" dal menu \"Tool\" per tornare nella"
  //: /line:"modalità  base di edizione. Seleziona un'altra posizione nell'area"
  //: /line:"di disegno e crea una nuova istanza del modulo \"foo\". Nota che"
  //: /line:"le informazioni sulle porte e sulla dimensione del blocco non"
  //: /line:"vengono riportate."
  //: /line:""
  //: /line:"8) Seleziona l'istanza di \"foo\" a cui hai aggiunto le porte"
  //: /line:"e attiva la voce \"Set Interface\" del menu \"Module\"."
  //: /line:"Crea un'altra istanza del modulo \"foo\". Ora il modulo \"foo\" "
  //: /line:"viene riprodotto con le sue porte e la dimensione "
  //: /line:"che gli hai dato."
  //: /line:""
  //: /line:"9) \"Apri\" il modulo \"foo\": seleziona una delle istanze e scegli"
  //: /line:"\"Open\" dal menu \"Module\" (oppure clicca doppio su \"foo\" nell'elenco"
  //: /line:"di moduli a sinistra della finestra principale)."
  //: /end
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
  //: comment g21 /dolink:1 /link:"@T/edit2_tut.v" @(630,85) /sn:0 /anc:1
  //: /line:"Gruppi di oggetti"
  //: /end
  //: comment g22 /dolink:0 /link:"" @(605,115) /sn:0 /anc:1
  //: /line:"->"
  //: /end
  //: comment g0 /dolink:0 /link:"" @(22,9) /anc:1
  //: /line:"Questo tutorial descrive come creare ed usare moduli in TkGate."
  //: /end
  //: comment g18 /dolink:1 /link:"@T/welcome_tut.v" @(630,40) /sn:0 /anc:1
  //: /line:"Introduzione a TkGate"
  //: /end

endmodule
