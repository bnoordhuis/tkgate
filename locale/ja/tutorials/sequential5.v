//: version "2.0"
//: property prefix = "_GG"
//: property title = "seqsim_tut.v"
//: property discardChanges = 1

//: /netlistBegin PAGE1
module PAGE1;    //: root_module
reg [7:0] w7;    //: /sn:0 {0}(#:199,201)(199,227){1}
supply0 w3;    //: /sn:0 {0}(252,251)(252,241)(239,241){1}
supply0 w0;    //: /sn:0 {0}(254,296)(274,296)(274,307){1}
reg clr;    //: {0}(32:254,286)(285,286){1}
wire [7:0] w2;    //: /sn:0 {0}(#:215,281)(215,256){1}
wire ck;    //: {0}(50:178,291)(113,291){1}
wire [7:0] reg_out;    //: {0}(#:215,302)(215,328)(273,328){1}
//: {2}(277,328)(333,328)(52:333,215)(231,215)(231,227){3}
//: {4}(275,330)(275,338){5}
wire w9;    //: /sn:0 {0}(191,241)(181,241){1}
//: enddecls

  //: joint g8 (reg_out) @(275, 328) /w:[ 2 -1 1 4 ]
  //: GROUND g4 (w0) @(274,313) /sn:0 /w:[ 1 ]
  //: comment g13 @(14,12)
  //: /line:"<h3>Sequential Simulation</h3> <b>(scripts)</b>"
  //: /line:""
  //: /line:"In addition to interactive simulator control, you can also write a simulator script."
  //: /line:"Simulator script files have the extension \".vs\" and are written using Verilog"
  //: /line:"syntax.  While the simulator is active, press <img src=sim_script.gif> to load a simulator script file."
  //: /line:"You can use a script file to set and remove probes, advance the simulation time,"
  //: /line:"set signal values and display messages in the TkGate log (<img src=log.gif>) window.  See"
  //: /line:"the TkGate 2.0 documentation for details on how to write simulator scripts."
  //: /end
  _GGREG8 #(10, 10, 20) g2 (.Q(reg_out), .D(w2), .EN(w0), .CLR(clr), .CK(ck));   //: @(215,291) /sn:0 /w:[ 0 0 0 0 0 ]
  //: comment g1 @(10,410) /sn:0 /R:14 /anc:1
  //: /line:"<tutorial-navigation byfile=1>"
  //: /end
  //: comment g10 @(495,118) /sn:0 /R:14 /anc:1
  //: /line:"Simulator Script Example:"
  //: /line:"<font face=courier>"
  //: /line:"initial"
  //: /line:"  begin"
  //: /line:"    $display(\"%t: Start of script\",$time);"
  //: /line:"    $tkg$probe(ck, reg_out, clr);"
  //: /line:"    $display(\"%t: Set probes\",$time);"
  //: /line:"    clr = 1'b0;"
  //: /line:"    #200;"
  //: /line:"    clr = 1'b1;"
  //: /line:"    $display(\"%t: Done with reset\",$time);"
  //: /line:"    #500;"
  //: /line:"    $stop();"
  //: /line:"  end"
  //: /line:"</font>"
  //: /end
  //: DIP g6 (w7) @(199,191) /sn:0 /w:[ 0 ] /st:1
  //: GROUND g9 (w3) @(252,257) /sn:0 /w:[ 0 ]
  //: LED g7 (reg_out) @(275,345) /sn:0 /R:2 /w:[ 5 ] /type:2
  _GGADD8 #(68, 70, 62, 64) g5 (.A(w7), .B(reg_out), .S(w2), .CI(w3), .CO(w9));   //: @(215,243) /sn:0 /w:[ 1 3 1 1 0 ]
  _GGCLOCK_P100_0_50 g0 (.Z(ck));   //: @(100,291) /sn:0 /w:[ 1 ] /omega:100 /phi:0 /duty:50
  //: SWITCH reset (clr) @(303,286) /R:2 /w:[ 1 ] /st:1

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

//: /builtinBegin
module _GGCLOCK_P100_0_50 (Z);
output Z;
reg Z;

initial #0
  begin 
    forever
      begin
        Z = 1'b0;
        #50;
        Z = 1'b1;
        #50;
      end
   end

endmodule
//: /builtinEnd
