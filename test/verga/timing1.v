module top;
  integer i;
   reg 	  ck;
  reg data,en;
  wire out;

  initial
    $monitor("%4d: ck=%b data=%b en=%b - out=%b",$time,ck,data,en,out);

  always
    #50 ck = !ck;

  initial
    begin
      ck = 1'b1;
      data = 1'b0; en = 1'b1;


      #95;
      data = 1'b1; en = 1'b1;
      #5;
      $display("Expect $setup violation at time %d.",$time); 

      #85;
      data = 1'b0; en = 1'b1;
      #15;
      $display("Expect no violations at time %d.",$time); 

      #91; 
      data = 1'b1; en = 1'b1;
      #9;
      $display("Expect $setup violation at time %d.",$time); 

      #50; 
      data = 1'b0; en = 1'b1;
      #50;
      $display("Expect no violations at time %d.",$time); 

      #95; 
      data = 1'b1; en = 1'b0;
      #5;
      $display("Expect no violations at time %d.",$time); 

      #2; 
      $display("Expect no violations at time %d.",$time); 
      data = 1'b1; en = 1'b1;
      #98;

      #2;
      $display("Expect $hold violation at time %d.",$time); 
      data = 1'b0; en = 1'b1;
      #98;

      #11;
      $display("Expect no violation at time %d.",$time); 
      data = 1'b1; en = 1'b1;
      #89;

      $finish;	
    end

  latch l1(ck,data,en,out);

endmodule

module latch(ck,data,en,out);
input ck,data,en;
output out;
reg out = 1'bx;

  specify
    $setup(data, posedge ck &&& en, 10);
    $hold(posedge ck &&& en, data, 10);
  endspecify

  always @(posedge ck)
    if (en)
      begin
	 out = data;
      end

endmodule
