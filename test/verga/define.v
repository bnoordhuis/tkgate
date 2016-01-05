`define FOO 1234
`define BAR 1

`define REG4 reg [3:0]

`define BAR 1234 \
	More text here


`timescale  100 ns / 1 ns

module top;
  `REG4 r;

/////////////////////////////////////////////////////////////////////////////
`ifdef FOO
  initial $display("FOO-A-1 was included. [ok]");
`endif

`ifdef BAR
  initial $display("BAR-A-1 was included. [ok]");
`endif

`ifdef ARG
  initial $display("ARG-A-1 was included. [fail]");
`endif
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
`ifdef FOO
`ifdef ARG
  initial $display("ARG-B-1 was included. [fail]");

`ifdef BAR
  initial $display("BAR-B-1 was included. [fail]");
`else
  initial $display("BAR-B-2 was included. [fail]");
`endif

`else
  initial $display("ARG-B-2 was included. [ok]");

`ifdef BAR
  initial $display("BAR-B-3 was included. [ok]");
`else
  initial $display("BAR-B-4 was included. [fail]");
`endif

`endif

  initial $display("FOO-B-2 was included. [ok]");
`endif
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
`ifdef ARG
  initial $display("ARG-C-1 was included. [fail]");
`elseif FOO
  initial $display("FOO-C-1 was included. [ok]");
`elseif BAR
  initial $display("BAR-C-1 was included. [fail]");
`else
  initial $display("else-C-1 was included. [fail]");
`endif
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
`ifdef FOO
  initial $display("FOO-D-1 was included. [ok]");
`endif

`ifdef BAR
  initial $display("BAR-D-1 was included. [ok]");
`endif

`ifdef ARG
  initial $display("ARG-D-1 was included. [fail]");
`endif
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
`ifdef ARG
  initial $display("ARG-E-1 was included. [fail]");
`elseif XYZ
  initial $display("XYZ-E-1 was included. [fail]");
`elseif BAR
  initial $display("BAR-E-1 was included. [ok]");
`else
  initial $display("else-E-1 was included. [fail]");
`endif
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
`ifdef FOO
  initial $display("FOO-F-1 was included. [ok]");
`elseif BAR
  initial $display("BAR-F-1 was included. [fail]");
`elseif FOO
  initial $display("FOO-F-2 was included. [fail]");
`else
  initial $display("else-F-1 was included. [fail]");
`endif
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
`ifdef XYZ
  initial $display("XYZ-G-1 was included. [fail]");
`elseif DFG
  initial $display("DFG-G-1 was included. [fail]");
`elseif QWE
  initial $display("QWE-G-1 was included. [fail]");
`else
  initial $display("else-G-1 was included. [ok]");
`endif
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
`ifdef XYZ
  initial $display("XYZ-H-1 was included. [fail]");
`elseif FOO
  initial $display("FOO-H-1 was included. [ok]");
`ifdef BAR
  initial $display("BAR-H-1 was included. [ok]");
`ifdef FOO
  initial $display("FOO-H-2 was included. [ok]");
`else
  initial $display("FOO-H-3 was included. [fail]");
`endif
`else
  initial $display("BAR-H-2 was included. [fail]");
`ifdef FOO
  initial $display("FOO-H-4 was included. [fail]");
`else
  initial $display("FOO-H-5 was included. [fail]");
`endif
`endif
`elseif QWE
  initial $display("QWE-H-1 was included. [fail]");
`ifdef BAR
  initial $display("BAR-H-3 was included. [fail]");
`ifdef FOO
  initial $display("FOO-H-6 was included. [fail]");
`else
  initial $display("FOO-H-7 was included. [fail]");
`endif
`else
  initial $display("BAR-H-4 was included. [fail]");
`ifdef FOO
  initial $display("FOO-H-8 was included. [fail]");
`else
  initial $display("FOO-H-9 was included. [fail]");
`endif
`endif
`else
  initial $display("else-H-1 was included. [fail]");
`endif
/////////////////////////////////////////////////////////////////////////////


  initial 
    begin
      r=5;
      $display("FOO=%d  r=",`FOO,r);
    end
    

endmodule
