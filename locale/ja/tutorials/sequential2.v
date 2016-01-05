//: version "2.0"
//: property prefix = "_GG"
//: property title = "seqsim_tut.v"
//: property useExtBars = 0
//: property discardChanges = 1
//: require "timer"

//: /netlistBegin PAGE1
module PAGE1;    //: root_module
reg [7:0] w7;    //: /sn:0 {0}(#:190,196)(190,222){1}
reg w4;    //: /sn:0 {0}(245,281)(256,281)(256,269)(266,269){1}
supply0 w0;    //: /sn:0 {0}(245,291)(268,291)(268,306){1}
supply0 w3;    //: /sn:0 {0}(243,246)(243,236)(230,236){1}
wire [7:0] w2;    //: /sn:0 {0}(#:206,276)(206,251){1}
wire ck;    //: {0}(50:169,286)(104,286){1}
wire [7:0] reg_out;    //: {0}(#:206,297)(206,327)(263,327){1}
//: {2}(267,327)(323,327)(-70:323,200)(222,200)(222,222){3}
//: {4}(265,329)(265,342){5}
wire w9;    //: /sn:0 {0}(182,236)(172,236){1}
//: enddecls

  //: GROUND g4 (w0) @(268,312) /sn:0 /w:[ 1 ]
  //: joint g8 (reg_out) @(265, 327) /w:[ 2 -1 1 4 ]
  //: comment g3 @(533,149) /sn:0 /anc:1
  //: /line:"<font color=green4>Hint: In addition to using the scroll bar"
  //: /line:"on the logic analyzer, you can click and"
  //: /line:"drag in the main window to fine tune the"
  //: /line:"the position of the traces.</font>"
  //: /end
  //: comment g13 @(14,12) /anc:1
  //: /line:"<h3>Sequential Simulation</h3> <b>(stepped simulation)</b>"
  //: /line:""
  //: /line:"Start the simulator by pressing the \"<img src=\"simstart.gif\"> <font color=red2>Simulate</a>\" tab just as with a combinational circuit.  Next, set probes"
  //: /line:"on the \"ck\" and \"reg_out\" signals by right clicking and selecting <font color=red2>Add/Delete</font> probe.  Now advance"
  //: /line:"the simulator by a several \"clock steps\" by pressing the <img src=sim_clock.gif> button or by pressing the <font color=red2>Tab</font> key.  Notice that"
  //: /line:"the \"reg_out\" single show an unknown value of \"x\".  In order to reset it and start the counter you,"
  //: /line:"must first set the \"reset\" switch to \"off\", advance the simulator by a clock step, then return"
  //: /line:"the \"reset\" switch to the \"on\" position."
  //: /end
  _GGREG8 #(10, 10, 20) g2 (.Q(reg_out), .D(w2), .EN(w0), .CLR(w4), .CK(ck));   //: @(206,286) /sn:0 /w:[ 0 0 0 0 0 ]
  //: comment g1 @(10,410) /sn:0 /R:14 /anc:1
  //: /line:"<tutorial-navigation byfile=1>"
  //: /end
  //: DIP g6 (w7) @(190,186) /sn:0 /w:[ 0 ] /st:1
  //: LED g7 (reg_out) @(265,349) /sn:0 /R:2 /w:[ 5 ] /type:2
  //: GROUND g9 (w3) @(243,252) /sn:0 /w:[ 0 ]
  _GGADD8 #(68, 70, 62, 64) g5 (.A(w7), .B(reg_out), .S(w2), .CI(w3), .CO(w9));   //: @(206,238) /sn:0 /w:[ 1 3 1 1 0 ]
  _GGCLOCK_P100_0_50 g0 (.Z(ck));   //: @(91,286) /sn:0 /w:[ 1 ] /omega:100 /phi:0 /duty:50
  //: SWITCH reset (w4) @(284,269) /R:2 /w:[ 1 ] /st:1

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
