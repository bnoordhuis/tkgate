module top;
  wire z;

  foo f1 ();
  foo #(4) f2();
  foo #(5) f3();

  OSCILLATOR g1(z);

  initial
    $monitor("z=%d",z);

endmodule

module foo #(.Z(2));
  parameter Z2 = Z/2;
  parameter Z3 = Z2*3;

  initial
    $display("Z=%d  Z2=%d",Z,Z2);

endmodule

module OSCILLATOR #(.HZ(1000)) (Z);
  parameter HZ0 = HZ/2;
  parameter HZ1 = HZ - HZ0;
  output Z;
  reg Z;
  time t;

  initial
    t = 0;

  always
    begin
      Z = 1'b0;
      if (t == 0) t = $tkg$systime;
      t = t + HZ0;
      $tkg$waituntil(t);
      Z = 1'b1;
      t = t + HZ1;
      $tkg$waituntil(t);
    end
  

endmodule
