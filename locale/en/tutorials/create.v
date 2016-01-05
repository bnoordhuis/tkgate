//: version "2.0-b6"
//: property encoding = "iso8859-1"
//: property locale = "en"
//: property prefix = "_GG"
//: property title = "create_tut.v"
//: property useExtBars = 0
//: property discardChanges = 1

`timescale 1ns/1ns

//: /netlistBegin PAGE1
module PAGE1;    //: root_module
reg w3;    //: /sn:0 {0}(219,137)(154,137){1}
reg w9;    //: /sn:0 {0}(314,237)(213,237){1}
//: {2}(211,235)(211,142)(219,142){3}
//: {4}(209,237)(154,237){5}
wire w8;    //: /sn:0 {0}(314,232)(294,232)(294,140)(240,140){1}
wire w10;    //: /sn:0 {0}(335,235)(345,235){1}
//: enddecls

  //: comment g8 @(313,208)
  //: /line:"OR gate"
  //: /end
  //: joint g13 (w9) @(211, 237) /w:[ 1 2 4 -1 ]
  //: comment g1 @(10,310) /sn:0 /anc:1
  //: /line:"<tutorial-navigation>"
  //: /end
  _GGAND2 #(6) g11 (.I0(w3), .I1(w9), .Z(w8));   //: @(230,140) /sn:0 /w:[ 0 3 1 ]
  //: comment g6 @(115,104)
  //: /line:"Switches"
  //: /end
  //: comment g7 @(208,112)
  //: /line:"AND gate"
  //: /end
  //: comment g9 @(10,10) /anc:1
  //: /line:"<h3>Creating a Simple Circuit</h3>"
  //: /line:""
  //: /line:"In this chapter, we will walk through the steps of creating the simple circuit shown below."
  //: /end
  //: SWITCH g14 (w3) @(137,137) /sn:0 /w:[ 1 ] /st:0
  _GGOR2 #(6) g0 (.I0(w8), .I1(w9), .Z(w10));   //: @(325,235) /sn:0 /w:[ 0 0 0 ]
  //: SWITCH g12 (w9) @(137,237) /sn:0 /w:[ 5 ] /st:0

endmodule
//: /netlistEnd

//: /netlistBegin PAGE6
module PAGE6();
//: interface  /sz:(40, 40) /bd:[ ]
reg w6;    //: /sn:0 {0}(154,237)(182,237)(182,237)(314,237){1}
reg w2;    //: /sn:0 {0}(219,137)(186,137)(186,137)(154,137){1}
wire w7;    //: /sn:0 {0}(335,235)(345,235){1}
wire w0;    //: /sn:0 {0}(204,142)(219,142){1}
wire w5;    //: /sn:0 {0}(314,232)(294,232)(294,140)(240,140){1}
//: enddecls

  _GGOR2 #(6) g4 (.I0(w5), .I1(w6), .Z(w7));   //: @(325,235) /sn:0 /w:[ 0 1 0 ]
  _GGAND2 #(6) g3 (.I0(w2), .I1(w0), .Z(w5));   //: @(230,140) /sn:0 /w:[ 0 1 1 ]
  //: SWITCH g2 (w6) @(137,237) /sn:0 /w:[ 0 ] /st:0
  //: comment g10 @(10,310) /sn:0 /R:14 /anc:1
  //: /line:"<tutorial-navigation>"
  //: /end
  //: comment g6 @(537,46) /sn:0 /anc:1
  //: /line:"<img src=\"makejoint_example.gif\">"
  //: /end
  //: comment g9 @(10,10) /anc:1
  //: /line:"<h3>Creating a Simple Circuit</h3>"
  //: /line:""
  //: /line:"Now make the final connection.  Grab the lower input terminal of the AND gate"
  //: /line:"and drag it down near the wire between the lower switch and the OR gate.  Release"
  //: /line:"the wire to create a joint."
  //: /line:""
  //: /end
  //: comment g7 @(537,126) /sn:0 /anc:1
  //: /line:"<font color=green4>Hint: Joints are formed any"
  //: /line:"time the soldering iron is"
  //: /line:"active and you release the"
  //: /line:"wire in the middle of another"
  //: /line:"wire.</font>"
  //: /end
  //: SWITCH g5 (w2) @(137,137) /sn:0 /w:[ 1 ] /st:0

endmodule
//: /netlistEnd

//: /netlistBegin PAGE3
module PAGE3();
//: interface  /sz:(40, 40) /bd:[ ]
wire w0;    //: /sn:0 {0}(204,137)(219,137){1}
wire w1;    //: /sn:0 {0}(204,142)(219,142){1}
wire w2;    //: /sn:0 {0}(240,140)(255,140){1}
//: enddecls

  _GGAND2 #(6) g2 (.I0(w0), .I1(w1), .Z(w2));   //: @(230,140) /sn:0 /w:[ 1 1 0 ]
  //: frame g1 @(283,202) /sn:0 /wi:78 /ht:46 /tx:""
  //: comment g10 @(10,310) /sn:0 /R:14 /anc:1
  //: /line:"<tutorial-navigation>"
  //: /end
  //: comment g9 @(10,10) /anc:1
  //: /line:"<h3>Creating a Simple Circuit</h3>"
  //: /line:""
  //: /line:"Now create an OR gate.  Right click in the dashed line box then select"
  //: /line:"<font color=red2>Make &rarr;   Gate &rarr;   OR</font> from the popup menu."
  //: /end
  //: comment g0 @(532,85) /sn:0 /anc:1
  //: /line:"<font color=green4>Hint: You can also use the keyboard"
  //: /line:"shortcut.  Select the position by left clicking"
  //: /line:"then use \"o\" to create an OR gate.</font>"
  //: /end

endmodule
//: /netlistEnd

//: /netlistBegin PAGE5
module PAGE5();
//: interface  /sz:(40, 40) /bd:[ ]
reg w6;    //: /sn:0 {0}(154,237)(169,237){1}
reg w2;    //: /sn:0 {0}(154,137)(169,137){1}
wire w7;    //: /sn:0 {0}(240,140)(255,140){1}
wire w4;    //: /sn:0 {0}(204,137)(219,137){1}
wire w3;    //: /sn:0 {0}(335,235)(350,235){1}
wire w0;    //: /sn:0 {0}(299,232)(314,232){1}
wire w1;    //: /sn:0 {0}(299,237)(314,237){1}
wire w5;    //: /sn:0 {0}(204,142)(219,142){1}
//: enddecls

  //: SWITCH g4 (w2) @(137,137) /sn:0 /anc:1 /w:[ 0 ] /st:0
  _GGAND2 #(6) g3 (.I0(w4), .I1(w5), .Z(w7));   //: @(230,140) /sn:0 /anc:1 /w:[ 1 1 0 ]
  //: SWITCH g2 (w6) @(137,237) /sn:0 /anc:1 /w:[ 0 ] /st:0
  _GGOR2 #(6) g1 (.I0(w0), .I1(w1), .Z(w3));   //: @(325,235) /sn:0 /anc:1 /w:[ 1 1 0 ]
  //: comment g10 @(10,310) /sn:0 /R:14 /anc:1
  //: /line:"<tutorial-navigation>"
  //: /end
  //: comment g6 @(578,122) /sn:0 /anc:1
  //: /line:"<font color=green4>Hint: The soldering iron cursor will"
  //: /line:"show you when you have selected"
  //: /line:"a connectable wire end.</font>"
  //: /end
  //: comment g9 @(10,10) /anc:1
  //: /line:"<h3>Creating a Simple Circuit</h3>"
  //: /line:""
  //: /line:"Finally, connect the wires.  Begin by grabbing the end of the wire leading from one of the"
  //: /line:"switches with the left mouse button and drag it to the upper input terminal of the AND gate."
  //: /end
  //: comment g0 @(579,61) /sn:0 /anc:1
  //: /line:"<img src=\"connect_example.gif\">"
  //: /end

endmodule
//: /netlistEnd

//: /netlistBegin PAGE2
module PAGE2();
//: interface  /sz:(40, 40) /bd:[ ]
//: enddecls

  //: comment g2 @(531,86) /sn:0 /anc:1
  //: /line:"<font color=green4>Hint: You can also use the keyboard"
  //: /line:"shortcut.  Select the position by left clicking"
  //: /line:"then use \"a\" to create an AND gate.</font>"
  //: /end
  //: frame g1 @(208,124) /sn:0 /wi:76 /ht:43 /tx:""
  //: comment g9 @(10,10) /anc:1
  //: /line:"<h3>Creating a Simple Circuit</h3>"
  //: /line:""
  //: /line:"Start by creating an AND gate.  Right click in the dashed line box then select"
  //: /line:"<font color=red2>Make &rarr;   Gate &rarr;   AND</font> from the popup menu."
  //: /end
  //: comment g0 @(10,310) /sn:0 /R:14 /anc:1
  //: /line:"<tutorial-navigation>"
  //: /end

endmodule
//: /netlistEnd

//: /netlistBegin PAGE4
module PAGE4();
//: interface  /sz:(40, 40) /bd:[ ]
wire w7;    //: /sn:0 {0}(335,235)(350,235){1}
wire w4;    //: /sn:0 {0}(299,232)(314,232){1}
wire w0;    //: /sn:0 {0}(204,137)(219,137){1}
wire w3;    //: /sn:0 {0}(240,140)(255,140){1}
wire w1;    //: /sn:0 {0}(204,142)(219,142){1}
wire w5;    //: /sn:0 {0}(299,237)(314,237){1}
//: enddecls

  _GGOR2 #(6) g4 (.I0(w4), .I1(w5), .Z(w7));   //: @(325,235) /sn:0 /anc:1 /w:[ 1 1 0 ]
  //: frame g8 @(109,215) /sn:0 /anc:1 /wi:76 /ht:43 /tx:""
  _GGAND2 #(6) g3 (.I0(w0), .I1(w1), .Z(w3));   //: @(230,140) /sn:0 /anc:1 /w:[ 1 1 0 ]
  //: frame g1 @(109,121) /sn:0 /anc:1 /wi:76 /ht:43 /tx:""
  //: comment g10 @(10,310) /sn:0 /R:14 /anc:1
  //: /line:"<tutorial-navigation>"
  //: /end
  //: comment g9 @(10,10) /anc:1
  //: /line:"<h3>Creating a Simple Circuit</h3>"
  //: /line:""
  //: /line:"Next create two switches.  Right click in each of the two dashed line box then"
  //: /line:"select <font color=red2>Make &rarr;   I/O &rarr;   Switch</font> from the popup menu."
  //: /end
  //: comment g0 @(532,86) /sn:0 /anc:1
  //: /line:"<font color=green4>Hint: You can also use the keyboard"
  //: /line:"shortcut.  Select the position by left clicking"
  //: /line:"then use \"s\" to create a switch.</font>"
  //: /end

endmodule
//: /netlistEnd


`timescale 1ns/1ns


//: /builtinBegin
module _GGAND2 #(.Diz(1)) (I0, I1,  Z);
   output  Z;
   input   I0, I1;
   reg  Z;

   assign #Diz Z = ( I0 & I1 );
   
endmodule // and
//: /builtinEnd


//: /builtinBegin
module _GGOR2 #(.Diz(1)) (I0, I1,  Z);
   output  Z;
   input   I0, I1;

     assign #Diz Z = ( I0 | I1 );
   
endmodule // or
//: /builtinEnd

