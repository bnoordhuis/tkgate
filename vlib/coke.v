//: version "2.0-b6"
//: property encoding = "iso8859-1"
//: property locale = "en"
//: property prefix = "_GG"
//: property title = "Coke Machine Interface"

`timescale 1ns/1ns

//: /netlistBegin main
module main;    //: root_module
wire w6;    //: /sn:0 {0}(57,149)(42,149){1}
wire w7;    //: /sn:0 {0}(57,165)(42,165){1}
wire w16;    //: /sn:0 {0}(240,117)(255,117){1}
wire [7:0] w14;    //: /sn:0 {0}(#:240,85)(255,85){1}
wire [2:0] w19;    //: /sn:0 {0}(#:240,165)(255,165){1}
wire w15;    //: /sn:0 {0}(240,101)(255,101){1}
wire w4;    //: /sn:0 {0}(57,117)(42,117){1}
wire w3;    //: /sn:0 {0}(57,101)(42,101){1}
wire [5:0] w0;    //: /sn:0 {0}(#:57,53)(42,53){1}
wire w21;    //: /sn:0 {0}(240,197)(255,197){1}
wire w24;    //: /sn:0 {0}(240,245)(255,245){1}
wire w23;    //: /sn:0 {0}(240,229)(255,229){1}
wire [2:0] w20;    //: /sn:0 {0}(#:240,181)(255,181){1}
wire [5:0] w1;    //: /sn:0 {0}(#:57,69)(42,69){1}
wire w25;    //: /sn:0 {0}(240,261)(255,261){1}
wire [2:0] w18;    //: /sn:0 {0}(#:240,149)(255,149){1}
wire w8;    //: /sn:0 {0}(57,181)(42,181){1}
wire w22;    //: /sn:0 {0}(240,213)(255,213){1}
wire w17;    //: /sn:0 {0}(240,133)(255,133){1}
wire [5:0] w12;    //: /sn:0 {0}(#:240,53)(255,53){1}
wire w11;    //: /sn:0 {0}(57,229)(42,229){1}
wire w2;    //: /sn:0 {0}(57,85)(42,85){1}
wire w10;    //: /sn:0 {0}(57,213)(42,213){1}
wire [5:0] w13;    //: /sn:0 {0}(#:240,69)(255,69){1}
wire w5;    //: /sn:0 {0}(57,133)(42,133){1}
wire w9;    //: /sn:0 {0}(57,197)(42,197){1}
wire w26;    //: /sn:0 {0}(240,277)(255,277){1}
//: enddecls

  cokemachine g0 (.INSACK(w11), .RETINS(w10), .CHGCOM(w9), .BILLOUT(w8), .BILLLT(w7), .BILLIN(w6), .NOCHG(w5), .CHGN(w4), .CHGD(w3), .CHGQ(w2), .ELIGHT(w1), .DISP(w0), .DISPACK(w26), .CHGACK(w25), .BILLACK(w24), .CNRET(w23), .BILLOK(w22), .BILLSNS(w21), .NUMN(w20), .NUMD(w19), .NUMQ(w18), .INSN(w17), .INSD(w16), .INSQ(w15), .COST(w14), .EMPTY(w13), .PRESS(w12));   //: @(58, 37) /sz:(181, 256) /sn:0 /p:[ Li0>0 Li1>0 Li2>0 Li3>0 Li4>0 Li5>0 Li6>0 Li7>0 Li8>0 Li9>0 Li10>0 Li11>0 Ro0<0 Ro1<0 Ro2<0 Ro3<0 Ro4<0 Ro5<0 Ro6<0 Ro7<0 Ro8<0 Ro9<0 Ro10<0 Ro11<0 Ro12<0 Ro13<0 Ro14<0 ]

endmodule
//: /netlistEnd

//: /hdlBegin cokemachine
//: interface  /sz:(181, 272) /bd:[ Li0>COST[7:0](208/272) Li1>INSACK(192/272) Li2>RETINS(176/272) Li3>CHGCOM(160/272) Li4>BILLOUT(144/272) Li5>BILLLT(128/272) Li6>BILLIN(112/272) Li7>NOCHG(96/272) Li8>CHGN(80/272) Li9>CHGD(64/272) Li10>CHGQ(48/272) Li11>ELIGHT[5:0](32/272) Li12>DISP[5:0](16/272) Bi0>_RESET(92/181) Ro0<BILLNG(240/272) Ro1<DISPACK(224/272) Ro2<CHGACK(208/272) Ro3<BILLACK(192/272) Ro4<CNRET(176/272) Ro5<BILLOK(160/272) Ro6<BILLSNS(144/272) Ro7<NUMN[2:0](128/272) Ro8<NUMD[2:0](112/272) Ro9<NUMQ[2:0](96/272) Ro10<INSN(80/272) Ro11<INSD(64/272) Ro12<INSQ(48/272) Ro13<EMPTY[5:0](32/272) Ro14<PRESS[5:0](16/272) ]
//: enddecls
module cokemachine(DISP, ELIGHT, CHGCOM, CHGQ, CHGD, CHGN, NOCHG, BILLIN, BILLOUT, BILLLT, RETINS, INSACK, EMPTY, PRESS, INSQ, INSD, INSN, BILLSNS, BILLOK, BILLNG, COST,CNRET, BILLACK, DISPACK, CHGACK);

  input [5:0] DISP;               // Dispense drink
  input [5:0] ELIGHT;             // Empty light
  input CHGCOM;                   // Commit change
  input CHGQ, CHGD, CHGN;         // Make change (quarter, dime, nickle)
  input NOCHG;                    // No change light
  input BILLIN, BILLOUT;          // Move bill in or out
  input BILLLT;                   // Insert bill light
  input RETINS;                   // Return inserted coins
  input INSACK;                   // Acknoeledge a coin insertion 
  input [7:0] COST;               // Cost per drink in nickles
  input _RESET;                   // Reset the coke machine.
  
  output [5:0] EMPTY, PRESS;      // Empty sensor, button press sensor
  output INSQ, INSD, INSN;        // Sense coin insertion (quarter, dime, nickle)
  output [2:0] NUMQ, NUMD, NUMN;  // Number of coins in changer (quarter, dime, nickle)
  output BILLSNS;                 // Sense bill in bill slot
  output BILLOK, BILLNG;          // Scanner reports bill ok (or no good)
  output CNRET;                   // Coin return pressed
  output BILLACK;                 // Acknowledge a BILLIN/BILLOUT signal
  output DISPACK;                 // Acknowledge a DISP signal
  output CHGACK;                  // Acknowledge a change signal

  reg [5:0] EMPTY, PRESS;
  reg INSQ, INSD, INSN;
  reg [2:0] NUMQ, NUMD, NUMN;
  reg BILLSNS;
  reg BILLOK, BILLNG;
  reg CNRET;
  reg BILLACK;
  reg DISPACK;
  reg CHGACK;
    
  initial
    begin
      CHGACK = 1'b0;
      DISPACK = 1'b0;
      $tkg$post("CokeMachine","%m");
      $tkg$exec("CokeMachine::setCost %m %d",COST);   
      $tkg$exec("CokeMachine::resetMachine %m %d",_RESET);   
    end

  //
  // Respon to changes in the "empty" light.
  //
  always @ (ELIGHT)
    $tkg$exec("CokeMachine::setEmpty %m {%b %b %b %b %b %b}", ELIGHT[0],ELIGHT[1],ELIGHT[2],ELIGHT[3],ELIGHT[4],ELIGHT[5]);

  //
  // Respond to changes in the "dispense" signal.
  //
  wire D0 = DISP[0];
  always @ (D0)
    if (D0 && !DISPACK)
      begin
        #10 $tkg$exec("CokeMachine::dispense %m 0");
        DISPACK = 1'b1;
      end
    else if (!D0)
      #10 DISPACK = 1'b0;

  wire D1 = DISP[1];
  always @ (D1)
    if (D1 && !DISPACK)
      begin
        #10 $tkg$exec("CokeMachine::dispense %m 1");
        DISPACK = 1'b1;
      end
    else if (!D1)
      #10 DISPACK = 1'b0;

  wire D2 = DISP[2];
  always @ (D2)
    if (D2 && !DISPACK)
      begin
        #10 $tkg$exec("CokeMachine::dispense %m 2");
        DISPACK = 1'b1;
      end
    else if (!D2)
      #10 DISPACK = 1'b0;

  wire D3 = DISP[3];
  always @ (D3)
    if (D3 && !DISPACK)
      begin
        #10 $tkg$exec("CokeMachine::dispense %m 3");
        DISPACK = 1'b1;
      end
    else if (!D3)
      #10 DISPACK = 1'b0;

  wire D4 = DISP[4];
  always @ (D4)
    if (D4 && !DISPACK)
      begin
        #10 $tkg$exec("CokeMachine::dispense %m 4");
        DISPACK = 1'b1;
      end
    else if (!D4)
      #10 DISPACK = 1'b0;

  wire D5 = DISP[5];
  always @ (D5)
    if (D5 && !DISPACK)
      begin
        #10 $tkg$exec("CokeMachine::dispense %m 5");
        DISPACK = 1'b1;
      end
    else if (!D5)
      #10 DISPACK = 1'b0;
  
  //       
  // Make change
  //       
  always @ (CHGQ) 
    if (CHGQ && !CHGACK)
      begin
        #10 $tkg$exec("CokeMachine::ejectChange %m 25");
        CHGACK = 1'b1;
      end
    else if (!CHGQ)
    #10 CHGACK = 1'b0;

  always @ (CHGD) 
    if (CHGD && !CHGACK)
      begin
        #10 $tkg$exec("CokeMachine::ejectChange %m 10");
        CHGACK = 1'b1;
      end
    else if (!CHGD)
      #10 CHGACK = 1'b0;

  always @ (CHGN) 
    if (CHGN && !CHGACK)
      begin
        #10 $tkg$exec("CokeMachine::ejectChange %m 5");
        CHGACK = 1'b1;
      end
    else if (!CHGN)
      #10 CHGACK = 1'b0;

  always @ (CHGCOM) 
    if (CHGCOM && !CHGACK)
      begin
        #10 $tkg$exec("CokeMachine::commitInserted %m");
        CHGACK = 1'b1;
      end
    else if (!CHGCOM)
      CHGACK = 1'b0;

  always @ (RETINS) 
    if (RETINS && !CHGACK)
      begin
        #10 $tkg$exec("CokeMachine::returnInserted %m");
        CHGACK = 1'b1;
      end
    else if (!RETINS)
      CHGACK = 1'b0;

  //
  // Acknowledge coin insertion
  //
  always @ (INSACK) $tkg$exec("CokeMachine::insertAck %m -value %b",INSACK);   


  always @ (COST) $tkg$exec("CokeMachine::setCost %m %d",COST);   
  always @ (_RESET) $tkg$exec("CokeMachine::resetMachine %m %d",_RESET);   

  //
  // No change, and insert bill lights
  //
//  always @ (NOCHG) $tkg$exec("CokeMachine::setNoChange %m %b",NOCHG);   
//  always @ (BILLLT) $tkg$exec("CokeMachine::setBillLight %m %b",BILLLT);
  always @ (NOCHG) $tkg$send("%m.NOCHG",NOCHG);
  always @ (BILLLT) $tkg$send("%m.BILLLT",BILLLT);



  //        
  // Bill reader handling   
  //        
  always @ (BILLIN)
     #10 $tkg$exec("CokeMachine::billIn %m -value %b",BILLIN);

  always @ (BILLOUT)
     #10 $tkg$exec("CokeMachine::billOut %m -value %b",BILLOUT);
  
  always #10 EMPTY = $tkg$recv("%m.EMPTY");
  always #10 PRESS = $tkg$recv("%m.PRESS");
  always #10 INSQ = $tkg$recv("%m.INSQ");
  always #10 INSD = $tkg$recv("%m.INSD");
  always #10 INSN = $tkg$recv("%m.INSN");
  always #10 NUMQ = $tkg$recv("%m.NUMQ");
  always #10 NUMD = $tkg$recv("%m.NUMD");
  always #10 NUMN = $tkg$recv("%m.NUMN");
  always #10 BILLSNS = $tkg$recv("%m.BILLSNS");
  always #10 CNRET = $tkg$recv("%m.CNRET");
  always #10 BILLOK = $tkg$recv("%m.BILLOK");
  always #10 BILLNG = $tkg$recv("%m.BILLNG");
  always #10 BILLACK = $tkg$recv("%m.BILLACK");

endmodule
//: /hdlEnd


`timescale 1ns/1ns


`timescale 1ns/1ns

