//: version "2.0-b6"
//: property encoding = "utf-8"
//: property locale = "de"
//: property prefix = "_GG"
//: property title = "texver_tut.v"
//: property discardChanges = 1

`timescale 1ns/1ns

//: /netlistBegin PAGE1
module PAGE1;    //: root_module
//: enddecls

  //: comment g7 @(10,310) /sn:0 /R:14 /anc:1
  //: /line:"<tutorial-navigation>"
  //: /end
  //: comment g9 @(8,9) /anc:1
  //: /line:"<h3>Textuelle Beschreibung mit Verilog </h3>"
  //: /line:""
  //: /line:"Der TkGate Simulator basiert auf der Hardwarebeschreibungssprache Verilog.  Jedes Modul, das"
  //: /line:"mit dem graphischen Editor entworfen wird, wird vor der Simulation in Verilog-Programmtext übersetzt."
  //: /line:"Man kann Module auch direkt in Verilog schreiben.  Allerdings kann dieser Abschnitt nicht dazu dienen,"
  //: /line:"die Sprache Verilog zu lehren. Vielmehr soll erläutert werden, wie man Verilog-Module in TkGate einbindet."
  //: /end
  //: comment g0 @(145,107) /sn:0 /anc:1
  //: /line:"<img src=bigtextedit.gif>"
  //: /end

endmodule
//: /netlistEnd

//: /hdlBegin foo
//: interface  /sz:(55, 80) /bd:[ Li0>d(64/80) Li1>c(48/80) Li2>b(32/80) Li3>a(16/80) Ro0<z(40/80) ]
//: enddecls
//
// Dies ist ein Beispiel eines Verilog Quelltext-Moduls.  Instanzen textueller
// Module koennen sowohl in graphischen als auch in textuellen Modulen benutzt
// werden und textuelle Module koennen Instanzen graphischer und textueller 
// Module includieren.
//
// Um zum Toplevelmodul zurueckzukehren, klicke rechts und waehle "Schliessen"
//
module foo(z,a,b,c,d);
output z;
input a,b,c,d;
reg z;
reg q;

  // Ordne a&b|c dem Register q an der Anstiegsflanke von d zu 
  always @ (posedge d)
    q = a&b | c;
  
  // Ordne q^c dem Register und Ausgang z an der Anstiegsflanke von d zu.
  always @ (posedge d)
    z = q ^ c; 

endmodule
//: /hdlEnd


`timescale 1ns/1ns

//: /netlistBegin PAGE2
module PAGE2();
//: interface  /sz:(40, 40) /bd:[ ]
reg w6;    //: /sn:0 {0}(163,145)(123,145)(123,136)(108,136){1}
reg w7;    //: /sn:0 {0}(163,161)(123,161)(123,169)(108,169){1}
reg w5;    //: /sn:0 {0}(108,101)(143,101)(143,129)(163,129){1}
wire w4;    //: /sn:0 {0}(220,153)(246,153)(246,138){1}
wire w8;    //: /sn:0 {0}(102,205)(145,205)(145,177)(163,177){1}
//: enddecls

  _GGCLOCK_P100_0_50 g4 (.Z(w8));   //: @(89,205) /sn:0 /w:[ 0 ] /omega:100 /phi:0 /duty:50
  //: SWITCH g3 (w7) @(91,169) /sn:0 /w:[ 1 ] /st:0
  //: SWITCH g2 (w6) @(91,136) /sn:0 /w:[ 1 ] /st:1
  //: SWITCH g1 (w5) @(91,101) /sn:0 /w:[ 0 ] /st:1
  //: comment g6 @(49,232) /anc:1
  //: /line:"2) Um ein neues Verilog-Quelltextmodul zu erzeugen, drücke <img src=\"blk_new.gif\"> und setze den Typ auf HDL."
  //: /end
  //: comment g9 @(8,9) /anc:1
  //: /line:"<h3>Textbasiertes Verilog</h3>"
  //: /line:""
  //: /line:"1) Klicke rechts auf das Modul unten und wähle <font color=blue>Öffnen</blue>."
  //: /end
  //: comment g7 @(10,310) /sn:0 /R:14 /anc:1
  //: /line:"<tutorial-navigation>"
  //: /end
  //: LED g5 (w4) @(246,131) /sn:0 /w:[ 1 ] /type:0
  foo g0 (.a(w5), .b(w6), .c(w7), .d(w8), .z(w4));   //: @(164, 113) /sz:(55, 80) /sn:0 /p:[ Li0>1 Li1>0 Li2>0 Li3>1 Ro0<0 ]

endmodule
//: /netlistEnd


`timescale 1ns/1ns


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

