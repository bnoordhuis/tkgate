//: version "2.0"
//: property title = "edit2_tut.v"
//: property useExtBars = 0
//: property discardChanges = 1

module main;    //: root_module
wire w3;    //: /sn:0 {0}(756,264)(679,264){1}
wire w1;    //: /sn:0 {0}(756,259)(712,259)(712,224)(679,224){1}
wire w2;    //: /sn:0 {0}(679,302)(689,302){1}
wire w5;    //: /sn:0 {0}(777,262)(787,262){1}
//: enddecls

  and g4 (.I0(w1), .I1(w3), .Z(w5));   //: @(767,262) /sn:0 /w:[ 0 0 0 ]
  //: comment g61 /dolink:1 /link:"@T/edwire_tut.v" @(622,100) /sn:0
  //: /line:"ワイヤの編集"
  //: /end
  //: switch g3 (w2) @(662,302) /sn:0 /w:[ 0 ] /st:0
  //: switch g2 (w3) @(662,264) /sn:0 /w:[ 1 ] /st:0
  //: switch g1 (w1) @(662,224) /sn:0 /w:[ 1 ] /st:0
  //: frame g11 @(434,347) /sn:0 /anc:1 /wi:373 /ht:230 /tx:"ゲートをこの中にコピーする"
  //: comment g16 /dolink:1 /link:"@T/module_tut.v" @(622,115) /sn:0
  //: /line:"モジュールの使い方"
  //: /end
  //: frame g10 @(22,347) /sn:0 /anc:1 /wi:394 /ht:227 /tx:"ゲートをこの中に移動する"
  //: comment g6 /dolink:1 /link:"@T/edit1_tut.v" @(622,70) /sn:0
  //: /line:"基本の編集モード"
  //: /end
  //: comment g9 /dolink:0 /link:"" @(607,452) /sn:0
  //: /line:""
  //: /end
  //: comment g7 /dolink:0 /link:"" @(634,193) /sn:0
  //: /line:""
  //: /end
  //: frame g15 @(600,25) /sn:0 /wi:224 /ht:149 /tx:"チュートリアル"
  //: comment g20 /dolink:1 /link:"@T/create_tut.v" @(622,55) /sn:0
  //: /line:"回路の作成"
  //: /end
  //: comment g17 /dolink:1 /link:"@T/sim_tut.v" @(622,130) /sn:0
  //: /line:"組合せ回路のシミュレーション"
  //: /end
  //: comment g5 /dolink:0 /link:"" @(38,36) /anc:1
  //: /line:"1) マウス左ボタンで右にある回路の左上をクリックして、箱が回路を完全に"
  //: /line:"含むまでマウスを右下へドラッグして、ボタンを離して下さい。"
  //: /line:"ゲートが選択され、太線で表示されます。選択されているゲートの集合を"
  //: /line:"セレクションといいます。"
  //: /line:""
  //: /line:"2) ゲートのどれかをクリックして「ゲートをこの中に移動する」と書いてある"
  //: /line:"点線の箱の中に移動してみて下さい。ゲートやワイヤのない所をクリックすると"
  //: /line:"セレクションは解除されます。"
  //: /line:""
  //: /line:"3) ANDゲートを選択して、マウス右ボタンを押し、「入力を追加」を選択して"
  //: /line:"下さい。それからANDゲートに追加されたワイヤを下のスイッチに接続して下さい。"
  //: /line:""
  //: /line:"4) ステップ1に書いてある様に全てのゲートを選択しておいて下さい。それから"
  //: /line:"「編集」メニューの「コピー」を選択し、マウス左ボタンで「ゲートをこの中に"
  //: /line:"コピーする」と書いてある点線の箱の真中をクリックして、「編集」メニューの"
  //: /line:"「張り付け」を選択して下さい。ゲートのセレクションがコピーされます。セレク"
  //: /line:"ションがある時、デリート・キーを押すと、セレクションのゲートが削除されます。"
  //: /line:""
  //: /line:"5) コントロール・キーを押しながら、ゲートをクリックすると、ゲートを一つずつ"
  //: /line:"セレクションに追加することが出来ます。"
  //: /end
  //: comment g14 /dolink:1 /link:"@T/seqsim_tut.v" @(622,145) /sn:0
  //: /line:"順序回路のシミュレーション"
  //: /end
  //: comment g21 /dolink:0 /link:"@T/edit2_tut.v" @(622,85) /sn:0 /anc:1
  //: /line:"グループの編集機能"
  //: /end
  //: comment g0 /dolink:0 /link:"" @(26,17) /anc:1
  //: /line:"このチュートリアルはグループ編集の機能を紹介する。"
  //: /end
  //: comment g22 /dolink:0 /link:"" @(605,85) /sn:0
  //: /line:"->"
  //: /end
  //: comment g18 /dolink:1 /link:"@T/welcome_tut.v" @(622,40) /sn:0
  //: /line:"TkGateの紹介"
  //: /end

endmodule
