#declare rootSeed = seed(42);
#macro Root(R,Ax,Az) 
      #local Tr = transform { rotate Ax*x rotate Az*z }
      #local R = 0.25;
      #local i = 0;
      #while ( R > 0.01)
        #local R = R - 0.002*rand(rootSeed);
	cylinder { 0*y, -0.01*y, R, 1 transform Tr }
        #local Ax = 0.7*rand(rootSeed);
        #local Az = 0.7*rand(rootSeed);
        #local Tr = transform { translate -0.01*y rotate Ax*x rotate Az*z transform Tr }
        #local i = i + 1;
      #end
#end

#declare Roots = 
  blob {
    threshold 0.01
    #local R = 0.25;
    #local A = 50;
    Root(R,A,A)
    Root(R,A,-A)
    Root(R,-A,A)
    Root(R,-A,-A)

    texture { Bark_Text }
  }
//  object { Roots translate -0.5*y }
