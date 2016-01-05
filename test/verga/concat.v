module top;
  reg [127:0] a0,a1,a2,a3,a4,a5,a6,b1,b2,b3;

  initial
    begin
       a0 = {4'h4, 2'h0, 2'h2};
       $display("a0=[%h]",a0);

       a1 = {17'h12,17'h12,17'h12};
       $display("a1=[%h]",a1);

       a2 = {16'h12,16'h12,16'h12};
       $display("a2=[%h]",a2);

       a3 = {15'h12,15'h12,15'h12};
       $display("a3=[%h]",a3);

       a4 = {8'h41,8'h42,8'h43};
       $display("a4=[%h]",a4);

       a5 = {11'h41,11'h42,11'h43};
       $display("a5=[%h]",a5);

       a6 = {32'h41,32'h42,32'h43};
       $display("a6=[%h]",a6);
       
       b1 = {3{17'h12}};
       $display("b1=[%h]",b1);

       b2 = {3{16'h12}};
       $display("b2=[%h]",b2);

       b3 = {3{15'h12}};
       $display("b3=[%h]",b3);
    end
endmodule
