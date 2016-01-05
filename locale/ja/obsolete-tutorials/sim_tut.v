//: version "2.0"
//: property title = "sim_tut.v"
//: property useExtBars = 0
//: property discardChanges = 1

module main;    //: root_module
wire w6;    //: /sn:0 {0}(750,326)(678,326){1}
wire w4;    //: /sn:0 {0}(652,260)(620,260){1}
//: {2}(618,258)(618,240)(588,240){3}
//: {4}(586,238)(586,227){5}
//: {6}(584,240)(571,240){7}
//: {8}(618,262)(618,323)(657,323){9}
wire w3;    //: /sn:0 {0}(687,225)(707,225){1}
//: {2}(709,223)(709,214){3}
//: {4}(709,227)(709,256){5}
//: {6}(711,258)(749,258){7}
//: {8}(709,260)(709,284){9}
wire w0;    //: /sn:0 {0}(750,321)(707,321)(707,305){1}
wire w1;    //: /sn:0 {0}(749,263)(706,263){1}
//: {2}(702,263)(673,263){3}
//: {4}(704,265)(704,284){5}
wire C;    //: /dp:1 {0}(798,312)(798,324)(771,324){1}
wire w5;    //: /sn:0 {0}(571,288)(585,288){1}
//: {2}(589,288)(607,288){3}
//: {4}(609,286)(609,265)(652,265){5}
//: {6}(609,290)(609,328)(657,328){7}
//: {8}(587,286)(587,276){9}
wire S;    //: {0}(770,261)(795,261)(795,251){1}
//: enddecls

  //: comment g61 /dolink:1 /link:"@T/edwire_tut.v" @(622,100) /sn:0
  //: /line:"ワイヤの編集"
  //: /end
  xor g4 (.I0(w4), .I1(w5), .Z(w1));   //: @(663,263) /sn:0 /w:[ 0 5 3 ]
  nand g8 (.I0(w0), .I1(w6), .Z(C));   //: @(761,324) /sn:0 /w:[ 0 0 1 ]
  //: comment g13 /dolink:0 /link:"" @(25,45) /anc:1
  //: /line:"1) Press the \"play\" button from the button bar above, or select"
  //: /line:"\"Begin Simulation\" from the \"Simulate\" menu.  The simulation"
  //: /line:"scope window should appear."
  //: /line:""
  //: /line:"2) Set a probe on the two on the two wires labeled \"S\" and \"C\""
  //: /line:"and on the wire leading out of each of the switches by double-"
  //: /line:"clicking on them.  A trace for each of the signals will appear"
  //: /line:"in the scope window."
  //: /line:""
  //: /line:"3) Hit the space bar several times to advance the simulation"
  //: /line:"time.  Each press of the space bar will advance the simulation"
  //: /line:"time by one epoch.  An epoch is the smallest unit of time"
  //: /line:"understood by the simulator."
  //: /line:""
  //: /line:"4) Select \"Options...\" from the \"File\" menu.  Click on the"
  //: /line:"\"Simulator\" tab in the options box and enter \"5\" in the entry"
  //: /line:"for \"Epoch Step Size\" and hit \"OK\".  Now try hitting the space"
  //: /line:"bar.  Each press will advance the simulator by 5 epochs."
  //: /line:""
  //: /line:"5) Click on one of the switches to change its value and hit the"
  //: /line:"space bar several times to advance the simulator.  The results"
  //: /line:"of the simulation should appear in the scope window."
  //: /line:""
  //: /line:"6) Press and hold the left mouse button on any of the internal"
  //: /line:"wires to view its value.  The value is displayed in \"Verilog\""
  //: /line:"format.  The first number indicates the bit width, the letter"
  //: /line:"indicates the radix, and the last number the value.  For"
  //: /line:"example, the \"1\" in \"1'b0\" indicates that this is a 1-bit value,"
  //: /line:"the \"b\" indicates the value is given in binary, and the \"0\""
  //: /line:"indicates the value of the signal."
  //: /line:""
  //: /line:"7) Press \"play\" button from the button bar again, or select \"Run\""
  //: /line:"from the \"Simulate\" menu.  This will cause the simulator to enter"
  //: /line:"a continuous simulation mode.  Try clicking on the switches in the"
  //: /line:"circuit.  Each time you change a switch, the simulation will"
  //: /line:"continue until the outputs are stable.  The \"Pause\" button will"
  //: /line:"return the simulator to single step mode."
  //: /line:""
  //: /line:"8) To end the simulation, press the \"stop\" button on the button"
  //: /line:"bar, or select \"End Simulation\" from the \"Simulate\" menu."
  //: /end
  //: switch g3 (w3) @(670,225) /sn:0 /w:[ 0 ] /st:1
  nand g2 (.I0(w4), .I1(w5), .Z(w6));   //: @(668,326) /sn:0 /w:[ 9 7 1 ]
  //: switch g1 (w5) @(554,288) /sn:0 /w:[ 0 ] /st:0
  //: comment g16 /dolink:1 /link:"@T/module_tut.v" @(622,115) /sn:0
  //: /line:"モジュールの使い方"
  //: /end
  //: joint g11 (w1) @(704, 263) /w:[ 1 -1 2 4 ]
  //: joint g10 (w3) @(709, 258) /w:[ 6 5 -1 8 ]
  led g28 (.I(w4));   //: @(586,220) /sn:0 /w:[ 5 ] /type:0
  //: comment g19 /dolink:1 /link:"@T/seqsim_tut.v" @(622,145) /sn:0
  //: /line:"順序回路のシミュレーション"
  //: /end
  //: joint g27 (w3) @(709, 225) /w:[ -1 2 1 4 ]
  //: joint g6 (w5) @(609, 288) /w:[ -1 4 3 6 ]
  xor g7 (.I0(w3), .I1(w1), .Z(S));   //: @(760,261) /sn:0 /w:[ 7 0 0 ]
  nand g9 (.I0(w1), .I1(w3), .Z(w0));   //: @(707,295) /sn:0 /R:3 /w:[ 5 9 1 ]
  //: comment g20 /dolink:1 /link:"@T/create_tut.v" @(622,55) /sn:0
  //: /line:"回路の作成"
  //: /end
  //: frame g15 @(600,25) /sn:0 /wi:224 /ht:149 /tx:"チュートリアル"
  //: joint g31 (w4) @(586, 240) /w:[ 3 4 6 -1 ]
  //: comment g17 /dolink:0 /link:"@T/sim_tut.v" @(622,130) /sn:0 /anc:1
  //: /line:"組合せ回路のシミュレーション"
  //: /end
  led g25 (.I(C));   //: @(798,305) /sn:0 /w:[ 0 ] /type:0
  led g29 (.I(w5));   //: @(587,269) /sn:0 /w:[ 9 ] /type:0
  //: comment g14 /dolink:1 /link:"@T/edit1_tut.v" @(622,70) /sn:0
  //: /line:"基本の編集モード"
  //: /end
  //: joint g5 (w4) @(618, 260) /w:[ 1 2 -1 8 ]
  //: comment g21 /dolink:1 /link:"@T/edit2_tut.v" @(622,85) /sn:0
  //: /line:"グループの編集機能"
  //: /end
  //: comment g24 /dolink:0 /link:"" @(500,0) /sn:0 /anc:1
  //: /line:"このページの翻訳は将来のバージョンに完成される。"
  //: /line:""
  //: /line:""
  //: /line:""
  //: /end
  //: comment g23 /dolink:1 /link:"@T/welcome_tut.v" @(622,40) /sn:0
  //: /line:"TkGateの紹介"
  //: /end
  //: comment g22 /dolink:0 /link:"" @(605,130) /sn:0
  //: /line:"->"
  //: /end
  //: switch g0 (w4) @(554,240) /sn:0 /w:[ 7 ] /st:1
  led g26 (.I(w3));   //: @(709,207) /sn:0 /w:[ 3 ] /type:0
  //: comment g12 /dolink:0 /link:"" @(16,18) /anc:1
  //: /line:"This tutorial introduces the fundamentals of simulating combinational circuits."
  //: /line:""
  //: /end
  led g18 (.I(S));   //: @(795,244) /sn:0 /w:[ 1 ] /type:0
  //: joint g30 (w5) @(587, 288) /w:[ 2 8 1 -1 ]

endmodule
