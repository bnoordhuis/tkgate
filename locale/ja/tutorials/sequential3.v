//: version "2.0"
//: property prefix = "_GG"
//: property title = "seqsim_tut.v"
//: property discardChanges = 1
//: require "timer"

//: /symbolBegin: 140311312
//: /iconBegin normal 815 5 5
//: /data "#define bitmap_width 28"
//: /data "#define bitmap_height 29"
//: /data "static unsigned char bitmap_bits[] = {"
//: /data "   0x03, 0x00, 0x00, 0x00, 0x0d, 0x00, 0x00, 0x00, 0x31, 0x00, 0x00, 0x00,"
//: /data "   0xc1, 0x00, 0x00, 0x00, 0x01, 0x03, 0x00, 0x00, 0x01, 0x0c, 0x00, 0x00,"
//: /data "   0x01, 0x30, 0x00, 0x00, 0x01, 0xc0, 0x00, 0x00, 0x01, 0x00, 0x03, 0x00,"
//: /data "   0x01, 0x00, 0x0c, 0x00, 0x01, 0x00, 0x30, 0x00, 0xf1, 0x3c, 0xc0, 0x00,"
//: /data "   0x91, 0x24, 0x00, 0x03, 0x91, 0x24, 0x00, 0x0c, 0x91, 0x24, 0x00, 0x08,"
//: /data "   0x91, 0x24, 0x00, 0x0c, 0x91, 0x24, 0x00, 0x03, 0x9d, 0xe7, 0xc0, 0x00,"
//: /data "   0x01, 0x00, 0x30, 0x00, 0x01, 0x00, 0x0c, 0x00, 0x01, 0x00, 0x03, 0x00,"
//: /data "   0x01, 0xc0, 0x00, 0x00, 0x01, 0x30, 0x00, 0x00, 0x01, 0x0c, 0x00, 0x00,"
//: /data "   0x01, 0x03, 0x00, 0x00, 0xc1, 0x00, 0x00, 0x00, 0x31, 0x00, 0x00, 0x00,"
//: /data "   0x0d, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00};"
//: /iconEnd
//: /iconBegin select 815 5 5
//: /data "#define bitmap_width 28"
//: /data "#define bitmap_height 29"
//: /data "static unsigned char bitmap_bits[] = {"
//: /data "   0x03, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x00,"
//: /data "   0xf3, 0x00, 0x00, 0x00, 0xc3, 0x03, 0x00, 0x00, 0x03, 0x0f, 0x00, 0x00,"
//: /data "   0x03, 0x3c, 0x00, 0x00, 0x03, 0xf0, 0x00, 0x00, 0x03, 0xc0, 0x03, 0x00,"
//: /data "   0x03, 0x00, 0x0f, 0x00, 0x03, 0x00, 0x3c, 0x00, 0xf3, 0x7d, 0xf0, 0x00,"
//: /data "   0xf3, 0x7d, 0xc0, 0x03, 0xb3, 0x6d, 0x00, 0x0f, 0xb3, 0x6d, 0x00, 0x0c,"
//: /data "   0xb3, 0x6d, 0x00, 0x0f, 0xb3, 0x6d, 0xc0, 0x03, 0xbf, 0xef, 0xf1, 0x00,"
//: /data "   0x9f, 0xe7, 0x3c, 0x00, 0x03, 0x00, 0x0f, 0x00, 0x03, 0xc0, 0x03, 0x00,"
//: /data "   0x03, 0xf0, 0x00, 0x00, 0x03, 0x3c, 0x00, 0x00, 0x03, 0x0f, 0x00, 0x00,"
//: /data "   0xc3, 0x03, 0x00, 0x00, 0xf3, 0x00, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x00,"
//: /data "   0x0f, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00};"
//: /iconEnd
//: /port output Z @(33, 19) /r:0
//: /symbolEnd

//: /netlistBegin PAGE1
module PAGE1;    //: root_module
reg [7:0] w7;    //: /sn:0 {0}(#:232,193)(232,219){1}
reg w4;    //: /sn:0 {0}(287,278)(298,278)(298,266)(308,266){1}
supply0 w3;    //: /sn:0 {0}(285,243)(285,233)(272,233){1}
supply0 w0;    //: /sn:0 {0}(287,288)(310,288)(310,303){1}
wire [7:0] w2;    //: /sn:0 {0}(#:248,273)(248,248){1}
wire ck;    //: {0}(-39:130,283)(211,283){1}
wire [7:0] reg_out;    //: {0}(#:248,294)(248,327)(304,327){1}
//: {2}(308,327)(364,327)(58:364,206)(264,206)(264,219){3}
//: {4}(306,329)(306,342){5}
wire w9;    //: /sn:0 {0}(224,233)(214,233){1}
//: enddecls

  //: joint g8 (reg_out) @(306, 327) /w:[ 2 -1 1 4 ]
  //: GROUND g4 (w0) @(310,309) /sn:0 /w:[ 1 ]
  //: comment g13 @(14,12) /anc:1
  //: /line:"<h3>Sequential Simulation</h3> <b>(continuous simulation)</b>"
  //: /line:""
  //: /line:"Also like with a combinational circuit, you can press the <img src=sim_go.gif> button to unpause and simulate"
  //: /line:"continously.  Try it with the circuit below.  This circuit contains a \"real-world-time\" oscilator"
  //: /line:"that will cycle once per second.  Like in the previous example, you will need to reset it by"
  //: /line:"turning the \"reset\" switch off monentarily."
  //: /end
  _GGREG8 #(10, 10, 20) g2 (.Q(reg_out), .D(w2), .EN(w0), .CLR(w4), .CK(ck));   //: @(248,283) /sn:0 /w:[ 0 0 0 0 1 ]
  //: comment g1 @(10,410) /sn:0 /R:14 /anc:1
  //: /line:"<tutorial-navigation byfile=1>"
  //: /end
  OSCILLATOR g10 (.Z(ck));   //: @(116, 283) /symbol:140311312 /sn:0 /w:[ 0 ]
  //: DIP g6 (w7) @(232,183) /sn:0 /w:[ 0 ] /st:1
  //: GROUND g9 (w3) @(285,249) /sn:0 /w:[ 0 ]
  //: LED g7 (reg_out) @(306,349) /sn:0 /R:2 /w:[ 5 ] /type:2
  _GGADD8 #(68, 70, 62, 64) g5 (.A(w7), .B(reg_out), .S(w2), .CI(w3), .CO(w9));   //: @(248,235) /sn:0 /w:[ 1 3 1 1 0 ]
  //: SWITCH reset (w4) @(326,266) /R:2 /w:[ 1 ] /st:1

endmodule
//: /netlistEnd

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
