module top;

  integer i,j;
  reg DSR;

  initial
    $tkg$command("exec TTY::post %m");

  always @(DSR)
    $tkg$command("exec TTY::data %m 8'h40");

endmodule

