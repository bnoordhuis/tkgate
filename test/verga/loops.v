module top #(.xyz(9));
  integer i;
  reg [7:0] a,b,c;
  reg [63:0] s;
  reg q;

  initial
    begin
      a = xyz;
      b = 5;
      c = a + b + 11;
      s = "abcd" + 8 ;

      $display("This is a hello world test string <%s> %d.","string tester",23);
      if (a > b)
        $display("if statment got a 'then' because %d > %d .",a,b);

      i = 0;
      while (i < 5)
        begin
          $display("while i = %d",i);
	  i = i + 1;
        end

      for (i = 3;i < 10;i = i + 1)
	begin
          $display("for i = %d",i);
        end

      $display("top done @%d.",$time);
    end 

  foo #(.delay(1)) f1();
  foo #(.delay(2)) f2();

endmodule

module foo #(.delay(1)) ();
  integer i;

  initial
    begin
      $display("starting %m with delay %d.",delay);
      for (i = 0;i <= 5;i = i + 1)
        #delay $display("in %m i = %d @ %d",i,$time);
    end


endmodule

module bar();
  reg [7:0] a,b,c; 
  parameter y = 8'h6;
  parameter q = {4'h5,4'h9,x,4'h3};
  parameter r = {6{1'b1}};
  parameter h = 13*3;
endmodule
