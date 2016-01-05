//: version "2.0"
//: property prefix = "_GG"
//: property title = "l2.v"

//: /netlistBegin main
module main;    //: root_module
wire [7:0] w6;    //: /sn:0 {0}#(478,266)(478,281){1}
wire w7;    //: /sn:0 {0}#(374,112)(389,112){1}
wire w4;    //: /sn:0 {0}#(409,529)(424,529){1}
wire [7:0] w3;    //: /sn:0 {0}#(478,302)#(478,373)(537,373){1}
wire w0;    //: /sn:0 {0}#(74,134)(89,134){1}
wire w1;    //: /sn:0 {0}#(74,139)(89,139){1}
wire w8;    //: /sn:0 {0}#(532,296)(517,296){1}
wire w2;    //: /sn:0 {0}#(338,109)(353,109){1}
wire w10;    //: /sn:0 {0}#(426,291)(441,291){1}
wire w5;    //: /sn:0 {0}#(445,527)(460,527){1}
wire w9;    //: /sn:0 {0}#(532,286)(517,286){1}
wire FOOBAR;    //: {0}#[50](424,524)(187,524)(187,304){1}
//: {2}(189,302)(234,302)(234,114)(353,114){3}
//: {4}[68](185,302)(119,302)(119,137)(110,137){5}
//: enddecls

  _GGREG8 #(10, 10, 20) g4 (.Q(w3), .D(w6), .EN(w8), .CLR(w9), .CK(w10));   //: @(478,291) /sn:0 /w:[ 0 1 1 1 1 ]
  //: joint g3 (FOOBAR) @(187, 302) /w:[ 2 -1 4 1 ]
  _GGAND2 #(6) g2 (.I0(w2), .I1(FOOBAR), .Z(w7));   //: @(364,112) /sn:0 /w:[ 1 3 0 ]
  _GGAND2 #(6) g1 (.I0(FOOBAR), .I1(w4), .Z(w5));   //: @(435,527) /sn:0 /w:[ 0 1 0 ]
  _GGAND2 #(6) g0 (.I0(w0), .I1(w1), .Z(FOOBAR));   //: @(100,137) /sn:0 /w:[ 1 1 5 ]

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
module _GGAND2 #(.Diz(1)) (I0, I1, Z);
output Z;
input I0;
input I1;

  assign #Diz Z = I0 & I1;

endmodule
//: /builtinEnd
