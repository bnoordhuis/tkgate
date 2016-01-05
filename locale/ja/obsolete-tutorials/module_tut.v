//: version "2.0"
//: property title = "module_tut.v"
//: property useExtBars = 0
//: property discardChanges = 1

module foo();
//: interface  /sz:(40, 40) /bd:[ ]
//: enddecls

  //: comment g61 @(622,100) /sn:0
  //: /line:"<a href=\"@T/edwire_tut.v\">ワイヤの編集</a>"
  //: /end
  //: comment g3 @(622,70) /sn:0
  //: /line:"<a href=\"@T/edit1_tut.v\">基本の編集モード</a>"
  //: /end
  //: frame g1 @(409,199) /sn:0 /wi:404 /ht:316 /tx:"Create Module Ports Here"
  //: comment g16 @(622,115) /sn:0
  //: /line:"モジュールの使い方"
  //: /end
  //: comment g20 @(622,55) /sn:0
  //: /line:"<a href=\"@T/create_tut.v\">回路の作成</a>"
  //: /end
  //: frame g15 @(600,25) /sn:0 /wi:224 /ht:149 /tx:"チュートリアル"
  //: comment g17 @(622,130) /sn:0
  //: /line:"<a href=\"@T/sim_tut.v\">組合せ回路のシミュレーション</a>"
  //: /end
  //: comment g14 @(622,145) /sn:0
  //: /line:"<a href=\"@T/seqsim_tut.v\">順序回路のシミュレーション</a>"
  //: /end
  //: comment g21 @(622,85) /sn:0
  //: /line:"<a href=\"@T/edit2_tut.v\">グループの編集機能</a>"
  //: /end
  //: comment g0 @(26,29) /anc:1
  //: /line:"10) This is the definition for the \"foo\""
  //: /line:"module.  The \"ports\" list in the lower left"
  //: /line:"corner of the main window indicates the"
  //: /line:"the ports that we defined on the default"
  //: /line:"module instance.  To create an internal"
  //: /line:"reference to a port, click the mouse to"
  //: /line:"set a mark and select \"Module Input\","
  //: /line:"\"Module Output\" or \"Module InOut\" from the"
  //: /line:"\"Module\" submenu of the \"Make\" menu.  A"
  //: /line:"dialog box will appear allowing you to set"
  //: /line:"the name of the port.  Enter a name and press"
  //: /line:"return."
  //: /line:""
  //: /line:"11) You can also create a new module"
  //: /line:"definition without creating an instance"
  //: /line:"by selecting \"New...\" from the \"Module\""
  //: /line:"menu.  This will bring up a dialog box"
  //: /line:"prompting for a module name.  Try entering"
  //: /line:"\"bar\" and pressing return."
  //: /line:""
  //: /line:"12) You can rename, copy and delete modules"
  //: /line:"using the other command in the \"Module\""
  //: /line:"menu."
  //: /line:""
  //: /line:"13) If you opened this block by double-clicking"
  //: /line:"on \"foo\" in the module list, you can return"
  //: /line:"to the main module by double-clicking on"
  //: /line:"\"main\". If you opened this block by using the"
  //: /line:"\"Open\" command from the \"Module\" menu, you"
  //: /line:"can return to the next level up by selecting"
  //: /line:"\"Close\" from the \"Module\" menu or by double-"
  //: /line:"clicking on main."
  //: /end
  //: comment g22 @(605,115) /sn:0
  //: /line:"->"
  //: /end
  //: comment g18 @(622,40) /sn:0
  //: /line:"<a href=\"@T/welcome_tut.v\">TkGateの紹介</a>"
  //: /end

endmodule

module main;    //: root_module
//: enddecls

  //: comment g61 @(622,100) /sn:0
  //: /line:"<a href=\"@T/edwire_tut.v\">ワイヤの編集</a>"
  //: /end
  //: comment g8 @(500,0) /sn:0 /anc:1
  //: /line:"このページの翻訳は将来のバージョンに完成される。"
  //: /end
  //: comment g3 @(622,70) /sn:0
  //: /line:"<a href=\"@T/edit1_tut.v\">基本の編集モード</a>"
  //: /end
  //: frame g2 @(477,189) /sn:0 /wi:330 /ht:350 /tx:"Use this space to create module instances"
  //: comment g1 @(11,34) /anc:1
  //: /line:"1) Click the left mouse button in the center of the blank area to the right of"
  //: /line:"this text block to set a position.  Now select \"Module Instance\" from the"
  //: /line:"\"Module\" submenu of the \"Make\" menu to create a module instance.  A dialog box"
  //: /line:"will appear with the entry for \"Function\" active.  Enter \"foo\" in lower case"
  //: /line:"and hit return to set the type of the instance to \"foo\"."
  //: /line:""
  //: /line:"2) Press the left mouse button near the lower right corner of the module"
  //: /line:"instance and drag it down and to the right to increase the size of the box."
  //: /line:""
  //: /end
  //: comment g16 @(622,115) /sn:0 /anc:1
  //: /line:"モジュールの使い方"
  //: /end
  //: comment g6 @(10,137) /anc:1
  //: /line:"3) Right click on the block near the left edge and select \"Add"
  //: /line:"Input\" from the popup menu.  A dialog box will appear with the"
  //: /line:"entry for \"Port Name\" active.  Type an \"A\" and press return."
  //: /line:"Now repeat the process to add input \"B\" and \"C\" on the same side."
  //: /line:"Repeat the process again this time selecting \"Add Output\" and"
  //: /line:"creating outputs \"X\", \"Y\" and \"Z\" on the right side of the block."
  //: /line:""
  //: /line:"4) Try clicking on one of the ports you added near the edge of"
  //: /line:"the module.  You can drag them along the edge they are attached"
  //: /line:"to change their position."
  //: /line:""
  //: /line:"5) Right click on the block near one of the ports and select"
  //: /line:"\"Change Type\" the popup menu.  This will change the direction"
  //: /line:"of the selected port.  You can repeat this action to cycle"
  //: /line:"through the three port types: input, output, and inout."
  //: /line:""
  //: /line:"6) Select \"Cut Wire\" from the \"Tool\" menu and click on the \"Z\""
  //: /line:"wire near the edge of the module instance.  This will delete"
  //: /line:"the port."
  //: /line:""
  //: /line:"7) Select \"Move/Connect\" from the \"Tool\" menu to return to the"
  //: /line:"default editing mode.  Now select a new position with the mouse"
  //: /line:"and create a new module instance named \"foo\".  Note that the"
  //: /line:"instance size and wire postions are not copied."
  //: /line:""
  //: /line:"8) Now select the instance of \"foo\" to which you added input"
  //: /line:"and output ports and select \"Set Interface\" from the \"Module\""
  //: /line:"menu.  Create another instance of a \"foo\" module.  Note that"
  //: /line:"the wire interface is now duplicated for each \"foo\" you create."
  //: /line:""
  //: /line:"9) Now \"open\" the \"foo\" module.  You can do this either by"
  //: /line:"selecting one of the \"foo\" module instances and then"
  //: /line:"selecting \"Open\" from the \"Module\" menu, or by double-clicking"
  //: /line:"on \"foo\" in the module list on the left side of the main window."
  //: /end
  //: comment g20 @(622,55) /sn:0
  //: /line:"<a href=\"@T/create_tut.v\">回路の作成</a>"
  //: /end
  //: frame g15 @(600,25) /sn:0 /wi:224 /ht:149 /tx:"チュートリアル"
  //: comment g17 @(622,130) /sn:0
  //: /line:"<a href=\"@T/sim_tut.v\">組合せ回路のシミュレーション</a>"
  //: /end
  //: comment g14 @(622,145) /sn:0
  //: /line:"<a href=\"@T/seqsim_tut.v\">順序回路のシミュレーション</a>"
  //: /end
  //: comment g21 @(622,85) /sn:0
  //: /line:"<a href=\"@T/edit2_tut.v\">グループの編集機能</a>"
  //: /end
  //: comment g22 @(605,115) /sn:0
  //: /line:"->"
  //: /end
  //: comment g0 @(22,9) /anc:1
  //: /line:"This tutorial describes the use of modules in TkGate."
  //: /end
  //: comment g18 @(622,40) /sn:0
  //: /line:"<a href=\"@T/welcome_tut.v\">TkGateの紹介</a>"
  //: /end

endmodule
