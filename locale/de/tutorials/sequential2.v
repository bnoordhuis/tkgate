//: version "2.0-b6"
//: property encoding = "utf-8"
//: property locale = "de"
//: property prefix = "_GG"
//: property title = "seqsim_tut.v"
//: property discardChanges = 1
//: require "timer"

`timescale 1ns/1ns

//: /netlistBegin PAGE1
module PAGE1;    //: root_module
reg [7:0] w7;    //: /sn:0 {0}(#:190,196)(190,222){1}
reg w4;    //: /sn:0 {0}(245,281)(256,281)(256,269)(266,269){1}
supply0 w3;    //: /sn:0 {0}(243,246)(243,236)(230,236){1}
supply0 w0;    //: /sn:0 {0}(245,291)(268,291)(268,306){1}
wire [7:0] w2;    //: /sn:0 {0}(#:206,276)(206,251){1}
wire ck;    //: {0}(50:169,286)(104,286){1}
wire [7:0] reg_out;    //: {0}(#:206,297)(206,327)(263,327){1}
//: {2}(267,327)(323,327)(-70:323,200)(222,200)(222,222){3}
//: {4}(265,329)(265,342){5}
wire w9;    //: /sn:0 {0}(182,236)(172,236){1}
//: enddecls

  //: joint g8 (reg_out) @(265, 327) /w:[ 2 -1 1 4 ]
  //: GROUND g4 (w0) @(268,312) /sn:0 /w:[ 1 ]
  //: comment g13 @(14,12) /anc:1
  //: /line:"<h3>Simulation von Schaltwerken</h3> <b>(Schrittweise Simulation)</b>"
  //: /line:""
  //: /line:"Starte den Simulator duch Drücken der \"<img src=\"simstart.gif\"> <font color=red2>Simulieren</a>\" Taste, genauso wie bei einem Schaltnetz.  Setze dann Tastköpfe"
  //: /line:"auf die \"ck\" und \"reg_out\" Signale durch Rechtsklicken und Auswahl von <font color=red2>Hinzufügen/Löschen</font> Tastkopf.  Schreite nun im"
  //: /line:"Simulator durch mehrere \"Taktschritte\" fort, indem Du den <img src=sim_clock.gif> Knopf oder die <font color=red2>Tab</font> Taste drückst.  Beachte, daß"
  //: /line:"das \"reg_out\" Signal einen unbekannten \"x\"-Wert zeigt. Um es zurückzusetzen und den Zähler neu zu starten, muß erst"
  //: /line:"der \"reset\" Schalter auf  \"off\" gesetzt werden. Rücke den Simulator um einen Taktschritt vor und stelle dann"
  //: /line:"den \"reset\" Schalter auf die \"on\" Position zurück."
  //: /end
  //: comment g3 @(500,149) /sn:0 /anc:1
  //: /line:"<font color=green4>Hinweis: Zusätzlich zum Scrollbar im"
  //: /line:"Logikanalysator kann man auch im Hauptfenster"
  //: /line:"durch Klicken und Ziehen die Position der Signale"
  //: /line:"feinabstimmen.</font>"
  //: /end
  _GGREG8 #(10, 10, 20) g2 (.Q(reg_out), .D(w2), .EN(w0), .CLR(w4), .CK(ck));   //: @(206,286) /sn:0 /w:[ 0 0 0 0 0 ]
  //: comment g1 @(10,410) /sn:0 /R:14 /anc:1
  //: /line:"<tutorial-navigation byfile=1>"
  //: /end
  //: DIP g6 (w7) @(190,186) /sn:0 /w:[ 0 ] /st:1
  //: GROUND g9 (w3) @(243,252) /sn:0 /w:[ 0 ]
  //: LED g7 (reg_out) @(265,349) /sn:0 /R:2 /w:[ 5 ] /type:2
  _GGADD8 #(68, 70, 62, 64) g5 (.A(w7), .B(reg_out), .S(w2), .CI(w3), .CO(w9));   //: @(206,238) /sn:0 /w:[ 1 3 1 1 0 ]
  _GGCLOCK_P100_0_50 g0 (.Z(ck));   //: @(91,286) /sn:0 /w:[ 1 ] /omega:100 /phi:0 /duty:50
  //: SWITCH reset (w4) @(284,269) /R:2 /w:[ 1 ] /st:1

endmodule
//: /netlistEnd


`timescale 1ns/1ns


//: /builtinBegin
module _GGREG8 #(.Dsetup(1), .Dhold(1), .Dck_q(1)) (Q, D, EN, CLR, CK);
  input CK,EN,CLR;
  input  [7:0] D;
  output  [7:0] Q;
  reg 	  [7:0] Qreg;
 
 // specify
   // $setup(D,posedge CK, Dsetup);
//    $hold(posedge CK,D, Dhold);
//  endspecify

  assign #Dck_q Q = Qreg;

  always @(posedge CK or negedge CLR)
    if (CLR === 1'b0)
      Qreg = 8'b0;
    else if (CK === 1'b1 && EN === 1'b0)
      Qreg = D;

endmodule
//: /builtinEnd


//: /builtinBegin
module _GGADD8 #(.Dab_s(1), .Dab_co(1), .Dci_s(1), .Dci_co(1)) (A, B, S, CI, CO);
   input  CI;
   output CO;
   input   [7:0] A,B;
   output  [7:0] S;
   wire    [7:0] _S;
   wire   _CO;
   
   specify
      (A,B *> S) = Dab_s;
      (A,B *> CO) = Dab_co;
      (CI *> S) = Dci_s;
      (CI *> CO) = Dci_co;
   endspecify

   assign {_CO,_S} = A + B + CI;

   assign CO =  _CO;
   assign S =  _S;

endmodule
//: /builtinEnd


//: /builtinBegin
module _GGCLOCK_P100_0_50 (Z);
   output Z;
   reg 	  Z =  1'b0;

   initial #50
     forever
       begin
	  Z =  1'b1;
	  #50;
	  Z =  1'b0;
	  #50;
       end
   
endmodule // clock
//: /builtinEnd

