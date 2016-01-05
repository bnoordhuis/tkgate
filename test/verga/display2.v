module top;

  reg [127:0] a;

  initial
    begin
	$display(1652763623);
	$display("%d",1652763623);
	$display(1652762343623232);
	$display("%d",1652762343623232);
	$display(1652762343623231827368712362);
	$display(98723498232342349823741652762343623231827368712362);
	a = 1652763623;
	$display(a);
    end

endmodule
