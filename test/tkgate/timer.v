//: version "2.0"
//: property prefix = "_GG"
//: property title = "timer.v"

//: /netlistBegin main
module main;    //: root_module
supply0 w0;    //: /sn:0 {0}(458,342)(458,326)(358,326){1}
supply0 w2;    //: /sn:0 {0}(357,271)(357,260)(343,260){1}
reg [7:0] w5;    //: /sn:0 {0}(255,203)(255,231)(303,231)(303,246){1}
wire w6;    //: /sn:0 {0}(184,321)(282,321){1}
wire [7:0] w7;    //: /sn:0 {0}(319,275)(319,311){1}
wire w4;    //: /sn:0 {0}(358,316)(582,316)(582,257)(525,257){1}
wire [7:0] w1;    //: /sn:0 {0}(319,332)(319,340){1}
//: {2}(321,342)(390,342)(390,219)(335,219)(335,246){3}
//: {4}(319,344)(319,425)(377,425)(377,412){5}
wire w9;    //: /sn:0 {0}(295,260)(280,260){1}
//: enddecls

  timer g4 (.Z(w6));   //: @(121, 296) /sz:(62, 63) /sn:0 /p:[ Ro0<0 ]
  //: joint g8 (w1) @(319, 342) /w:[ 2 1 -1 4 ]
  _GGADD8 #(68, 70, 62, 64) g3 (.A(w5), .B(w1), .S(w7), .CI(w2), .CO(w9));   //: @(319,262) /sn:0 /w:[ 1 3 0 1 0 ]
  //: GROUND g2 (w0) @(458,348) /sn:0 /w:[ 0 ]
  oneshot g1 (.Z(w4));   //: @(455, 232) /sz:(69, 63) /sn:0 /p:[ Ro0<1 ]
  //: DIP g6 (w5) @(255,193) /sn:0 /w:[ 0 ] /st:1
  //: LED g7 (w1) @(377,405) /sn:0 /w:[ 5 ] /type:2
  //: GROUND g5 (w2) @(357,277) /sn:0 /w:[ 0 ]
  _GGREG8 #(10, 10, 20) g0 (.Q(w1), .D(w7), .EN(w0), .CLR(w4), .CK(w6));   //: @(319,321) /sn:0 /w:[ 0 1 1 0 1 ]

endmodule
//: /netlistEnd

//: /hdlBegin timer
//: interface  /sz:(62, 63) /bd:[ Ro0<Z(25/63) ]
//: enddecls
module timer(Z);
  output Z;
  reg Z;
  
  initial
    begin
      Z = 0;
      forever
        begin
          $tkg$wait(250);
          Z = !Z;
        end
    end

endmodule
//: /hdlEnd

//: /hdlBegin oneshot
//: interface  /sz:(40, 40) /bd:[ ]
//: enddecls
module oneshot;
  output Z;
  reg Z;
  
  initial
    begin
      Z = 0;
      $tkg$wait(1000);
      Z = !Z;
    end

endmodule
//: /hdlEnd

//: /builtinBegin
module _GGREG8 #(.Dsetup(1), .Dhold(1), .Dck_q(1)) (Q, D, EN, CLR, CK);
input CK,EN,CLR;
input [7:0] D;
output [7:0] Q;
reg [7:0] Qreg;

  assign #Dck_q Q = Qreg;

  always @(posedge CK or negedge CLR)
    begin
    if (CLR === 1'b0)
      Qreg = 8'h0;
    else if (CK === 1'b1 && EN === 1'b0)
      Qreg = D;
    end

endmodule
//: /builtinEnd

//: /builtinBegin
module _GGADD8 #(.Dab_s(1), .Dab_co(1), .Dci_s(1), .Dci_co(1)) (A, B, S, CI, CO);
input CI;
output CO;
input [7:0] A,B;
output [7:0] S;

  specify
    (A,B *> S) = Dab_s;
    (A,B *> CO) = Dab_co;
    (CI *> S) = Dci_s;
    (CI *> CO) = Dci_co;
  endspecify

  assign {CO,S} = A + B + CI;

endmodule
//: /builtinEnd
