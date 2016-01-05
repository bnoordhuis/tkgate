module top;
   reg ck;

  specify
    $width(posedge ck, 30);
    $width(negedge ck, 25);
  endspecify

  initial
    $monitor("%4d: ck=%b",$time,ck);

  initial
    begin
        ck = 1'b1;
        #50;
	ck = 1'b0;
        $display("No violation expected at %d",$time);
        #50;

	ck = 1'b1;
        $display("No violation expected at %d",$time);
        #50;
	ck = 1'b0;
        $display("No violation expected at %d",$time);
        #50;

	ck = 1'b1;
        $display("No violation expected at %d",$time);
        #20;
	ck = 1'b0;
        $display("$width violation expected at %d on posedge",$time);
        #80;


	ck = 1'b1;
        $display("No violation expected at %d",$time);
        #90;
	ck = 1'b0;
        $display("No violation expected at %d",$time);
        #10;

	ck = 1'b1;
        $display("$width violation expected at %d on negedge",$time);
        #50;
	ck = 1'b0;
        $display("No violation expected at %d",$time);
        #50;

    end

endmodule

