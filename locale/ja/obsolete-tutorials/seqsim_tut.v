//: version "2.0"
//: property title = "seqsim_tut.v"
//: property useExtBars = 0
//: property discardChanges = 1

module main;    //: root_module
supply0 w3;    //: /sn:0 {0}(705,275)(705,265)(692,265){1}
supply0 w0;    //: /sn:0 /dp:1 {0}(707,320)(730,320)(730,335){1}
wire [7:0] w7;    //: /sn:0 {0}(652,225)(652,251){1}
wire w4;    //: /sn:0 /dp:1 {0}(707,310)(718,310)(718,298)(728,298){1}
wire [7:0] w2;    //: /sn:0 /dp:1 {0}(668,305)(668,280){1}
wire ck;    //: /dp:1 {0}(631,315)(566,315){1}
wire [7:0] reg_out;    //: {0}(668,326)(668,368)(724,368){1}
//: {2}(728,368)(784,368)(784,241)(684,241)(684,251){3}
//: {4}(726,370)(726,400){5}
wire w9;    //: /sn:0 {0}(644,265)(634,265){1}
//: enddecls

  //: joint g8 (reg_out) @(726, 368) /w:[ 2 -1 1 4 ]
  //: supply0 g4 (w0) @(730,341) /sn:0 /w:[ 1 ]
  //: comment g61 /dolink:1 /link:"@T/edwire_tut.v" @(622,100) /sn:0
  //: /line:"ワイヤの編集"
  //: /end
  //: switch g3 (w4) @(746,298) /sn:0 /R:2 /w:[ 1 ] /st:1
  //: comment g13 /dolink:0 /link:"" @(24,45) /anc:1
  //: /line:"1) Press the \"play\" button from the button bar above, or select"
  //: /line:"\"Begin Simulation\" from the \"Simulate\" menu.  The simulation"
  //: /line:"scope window should appear."
  //: /line:""
  //: /line:"2) Set a probe on the two on the clock line labled \"ck\" and on"
  //: /line:"the register output labeled \"reg_out\" by double-clicking on them."
  //: /line:"A trace for each of the signals will appear in the scope window."
  //: /line:""
  //: /line:"3) Hit the space bar several times to advance the simulation"
  //: /line:"time.  Each press of the space bar will advance the simulation"
  //: /line:"time by a set number of epochs.  An epoch is the smallest unit"
  //: /line:"of time understood by the simulator.  See the combinational"
  //: /line:"simulation tutorial for details on setting the step size."
  //: /line:""
  //: /line:"4) You can advance by a clock cycle by pressing the Tab key or"
  //: /line:"by selecting \"Step Cycle\" from the \"Simulate\" menu.  The simulation"
  //: /line:"will advance to the next positive edge on any clock in the circuit,"
  //: /line:"plus a number of \"overstep\" epochs.  The overstep is give registers"
  //: /line:"enough time to latch in the new values before pausing the simulation."
  //: /line:"You can change the overstep amount, change the number of cycles to"
  //: /line:"advance, or select a single clock to step on through the simulation"
  //: /line:"options dialog box."
  //: /line:""
  //: /line:"5) In this example, set switch on the CLR input to \"off\" and advance"
  //: /line:"the simulation by one cycle to clear the register.  Then turn the"
  //: /line:"switch to \"on\" and step the clock a few cycles to advance the counter."
  //: /line:""
  //: /line:"6) Click on the dip switch to open a window allowing you to change"
  //: /line:"the dip switch value.  Enter a number in hex and press \"Apply\" to"
  //: /line:"change the size of the step interval."
  //: /line:""
  //: /line:"7) Just as in the combinational logic example, you can click and hold"
  //: /line:"the mouse button on a wire to see the value on that wire."
  //: /line:""
  //: /line:"8) Press \"play\" button from the button bar again, or select \"Run\""
  //: /line:"from the \"Simulate\" menu.  This will cause the simulator to enter"
  //: /line:"a continuous simulation mode.  In sequential circuits, the simulator"
  //: /line:"will run continuously until you press the \"Pause\" button.  You can"
  //: /line:"still change the values of switches and such while the simulation"
  //: /line:"is running."
  //: /line:""
  //: /end
  register g2 (.Q(reg_out), .D(w2), .EN(w0), .CLR(w4), .CK(ck));   //: @(668,315) /sn:0 /w:[ 0 0 0 0 0 ]
  //: comment g1 /dolink:1 /link:"@T/edit1_tut.v" @(622,70) /sn:0
  //: /line:"基本の編集モード"
  //: /end
  //: comment g11 /dolink:0 /link:"" @(500,0) /sn:0 /anc:1
  //: /line:"このページの翻訳は将来のバージョンに完成される。"
  //: /line:""
  //: /line:""
  //: /line:""
  //: /end
  //: comment g16 /dolink:1 /link:"@T/module_tut.v" @(622,115) /sn:0
  //: /line:"モジュールの使い方"
  //: /end
  //: comment g10 /dolink:0 /link:"" @(528,451) /sn:0 /R:2 /anc:1
  //: /line:"9) You can slide the scope trace either"
  //: /line:"by using the scroll bar, or by clicking"
  //: /line:"in the trace window and dragging the"
  //: /line:"trace left and right.  Click while holding"
  //: /line:"the control or shift keys to zoom in or"
  //: /line:"out.  Dragging the scroll bar all the way"
  //: /line:"to the right will cause current values to"
  //: /line:"be continuously updated again."
  //: /line:""
  //: /end
  //: dip g6 (w7) @(652,215) /sn:0 /w:[ 0 ] /st:1
  //: supply0 g9 (w3) @(705,281) /sn:0 /w:[ 0 ]
  led g7 (.I(reg_out));   //: @(726,407) /sn:0 /R:2 /w:[ 5 ] /type:2
  //: frame g15 @(600,25) /sn:0 /wi:224 /ht:149 /tx:"チュートリアル"
  //: comment g20 /dolink:1 /link:"@T/create_tut.v" @(622,55) /sn:0
  //: /line:"回路の作成"
  //: /end
  //: comment g17 /dolink:1 /link:"@T/sim_tut.v" @(622,130) /sn:0
  //: /line:"組合せ回路のシミュレーション"
  //: /end
  add g5 (.A(w7), .B(reg_out), .S(w2), .CI(w3), .CO(w9));   //: @(668,267) /sn:0 /w:[ 1 3 1 1 0 ]
  //: comment g14 /dolink:0 /link:"@T/seqsim_tut.v" @(622,145) /sn:0 /anc:1
  //: /line:"順序回路のシミュレーション"
  //: /end
  //: comment g21 /dolink:1 /link:"@T/edit2_tut.v" @(622,85) /sn:0
  //: /line:"グループの編集機能"
  //: /end
  clock g0 (.Z(ck));   //: @(553,315) /sn:0 /w:[ 1 ] /omega:100 /phi:0 /duty:50
  //: comment g22 /dolink:0 /link:"" @(605,145) /sn:0
  //: /line:"->"
  //: /end
  //: comment g12 /dolink:0 /link:"" @(16,18) /anc:1
  //: /line:"This tutorial introduces the fundamentals of simulating sequential circuits."
  //: /line:""
  //: /end
  //: comment g18 /dolink:1 /link:"@T/welcome_tut.v" @(622,40) /sn:0
  //: /line:"TkGateの紹介"
  //: /end

endmodule
