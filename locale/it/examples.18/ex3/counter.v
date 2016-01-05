//: version "1.8.3"
//: property useExtBars = 0
//: property discardChanges = 1

module main;    //: root_module
supply0 w11;    //: /sn:0 {0}(269,197)(269,181)(248,181){1}
wire [7:0] w4;    //: /sn:0 {0}(208,114)(208,167){1}
wire w8;    //: /sn:0 {0}(263,269)(325,269)(325,283){1}
wire w12;    //: /sn:0 {0}(200,181)(190,181){1}
wire ck;    //: /dp:1 {0}(187,264)(56,264){1}
wire [7:0] w5;    //: /sn:0 {0}(224,196)(224,254){1}
wire [7:0] reg_out;    //: {0}(224,275)(224,332)(297,332)(297,173){1}
//: {2}(299,171)(379,171){3}
//: {4}(297,169)(297,145)(240,145)(240,167){5}
wire w9;    //: /sn:0 {0}(263,259)(324,259)(324,245){1}
//: enddecls

  //: comment g8 /dolink:0 /link:"" @(19,191)
  //: /line:"2) Aggancia una sonda"
  //: /line:"alla linea del clock"
  //: /line:"cliccando doppio"
  //: /line:"sul collegamento."
  //: /end
  clock g4 (.Z(ck));   //: @(43,264) /sn:0 /w:[ 1 ] /omega:100 /phi:0 /duty:50
  //: comment g13 /dolink:0 /link:"" @(23,412)
  //: /line:"7) Inserisci Tab per far avanzare la simulazione di un ciclo di clock."
  //: /line:"Siccome la linea CLR è attiva bassa, il registro viene azzerato."
  //: /line:"Imposta ora la linea CLR su \"on\" (commuta l'interruttore)"
  //: /line:"e continua la simulazione con il tasto Tab."
  //: /line:"Osserva il valore del registro che incrementa ad ogni ciclo di clock."
  //: /end
  //: dip g3 (w4) @(208,104) /sn:0 /w:[ 0 ] /st:1
  add g2 (.A(w4), .B(reg_out), .S(w5), .CI(w11), .CO(w12));   //: @(224,183) /sn:0 /w:[ 1 5 0 1 0 ]
  //: switch g1 (w8) @(325,297) /sn:0 /R:1 /w:[ 1 ] /st:0
  //: comment g11 /dolink:0 /link:"" @(348,218)
  //: /line:"5) Assicurati che "
  //: /line:"la linea CLR sia \"off\""
  //: /line:"per azzerare il registro."
  //: /end
  //: comment g10 /dolink:0 /link:"" @(251,90)
  //: /line:"4) Clicca sull'interruttore e imposta il valore 1"
  //: /line:"nel form che appare. Conferma con \"Apply\"."
  //: /end
  //: supply0 g6 (w11) @(269,203) /sn:0 /w:[ 0 ]
  //: comment g9 /dolink:0 /link:"" @(170,337)
  //: /line:"3) Aggancia una sonda "
  //: /line:"all'uscita del registro"
  //: /line:"cliccando doppio"
  //: /line:"sul collegamento."
  //: /end
  //: switch g7 (w9) @(324,232) /sn:0 /R:3 /w:[ 1 ] /st:0
  //: comment g15 /dolink:0 /link:"" @(25,501) /sn:0
  //: /line:"8) Modifica il valore dato dall'interruttore ad 8 vie: inserisci 2."
  //: /line:"Continua la simulazione per alcuni cicli di clock e osserva come"
  //: /line:"il registro ora incrementa a passi di 2."
  //: /end
  led g17 (.I(reg_out));   //: @(386,171) /sn:0 /R:3 /w:[ 3 ] /type:2
  //: joint g14 (reg_out) @(297, 171) /w:[ 2 4 -1 1 ]
  //: comment g5 /dolink:0 /link:"" @(15,11)
  //: /line:"Contatore a 8-Bit"
  //: /line:""
  //: /line:"1) Attiva la simulazione con \"Ctl-S b\""
  //: /line:""
  //: /end
  register g0 (.Q(reg_out), .D(w5), .EN(w8), .CLR(w9), .CK(ck));   //: @(224,264) /sn:0 /w:[ 0 1 0 0 0 ]
  //: comment g12 /dolink:0 /link:"" @(350,285)
  //: /line:"6) Assicurati che"
  //: /line:"la linea EN sia \"off\"."
  //: /end

endmodule
