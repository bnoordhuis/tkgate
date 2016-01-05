initial
  begin
    $monitor("%d: a=%d b=%d ci=%d : s=%d co=%d",$time,w19,w0,w11,w22,w24);
    w19 = 8'h0;
    w0 = 8'h5;
    #100 ;

    w19 = 8'h7;
    w0 = 8'h8;
    #100 ;
    #100 ;
  end
