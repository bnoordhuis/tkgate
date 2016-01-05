module top;
  real n,a,b,c;

  initial
    begin
	#1:2:3 $display("%t: done",$time);	
    end
endmodule
