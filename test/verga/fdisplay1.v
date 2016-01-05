module top;

  integer f,g;
  reg x;

  initial
    begin
      f = $fopen("test1.out");
      g = $fopen("test2.out");
      $display("f=%d",f);
      $fdisplay(1|f|g|16,"hello world");
      x = 0;
      # 1;
      $fstrobe(1|f|g,"x=%d",x);
      $fdisplay(1|f|g,"x=%d",x);
      x = 1;
      # 1;
      $fmonitor(1|f,"%d: monitored x = %d", $time, x);
      # 1;
      x = 0;
      # 3;
      x = 1;
      # 2;
      x = 0;
    end

endmodule
