module top;
   reg [7:0] m[0:255];
   reg [7:0] addr;
   reg x = 1'b0;
   integer i;
   wire [7:0] w;

   assign     w = m[3];

   always @(w)
     $display($time,": w changed to ",w);
   
   initial
     begin
	m[3][7:4] = 9;
	m[3][3:0] = 2;

	m[3] <= #2 7;
	m[3][7:4] <= #3 3;
	m[3] <= @(posedge x) 8'h99;

	$display($time,": m[3]=",m[3]," w=",w);
	for (i = 0;i < 20;i = i+1)
	  #1 $display($time,": m[3]=",m[3]," w=",w);
     end // initial begin

   initial
     begin
	#6 x = 1'b1;
	#3 x = 1'b0;
	m[3] = 8'h42;
	#3 x = 1'b1;
	#3 m[4] = 9;
     end
   
endmodule // top
