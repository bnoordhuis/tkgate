/****************************************************************************\
 *                                                                          *
 *                            The Towers of Hanoi                           *
 *                                                                          *
\****************************************************************************/

/*
 *  Setup up some global parameters, for ease of change.
 */

`define clk_period 20
`define setup_time 4
`define hold_time  1


/****************************************************************************\
 *                                                                          *
 *               'Basic building block' module definitions                  *
 *                                                                          *
\****************************************************************************/

module toggle(q, qbar, clk, toggle, reset);

   /*
    *  A mixed style model of a T-type (toggle) flip-flop,
    *  with a reset line and delays on the outputs.
    *  This first part is behavioural code.
    */

   output q, qbar;
   input  clk, toggle, reset;
   reg	  q;

   always @(posedge clk)
      if (reset == 1)
	 #5 q = 0;
      else if (toggle == 1)
	 #6 q = ~q;

   /*  This part is dataflow-style  */
   
   assign #1 qbar = ~q;

endmodule /* toggle */

module effr(q, clk, enable, reset, d);

   /*
    *  A behavioural model of an E-type (enable) flip-flop
    *  with a reset signal, and delays on the outputs.
    */

   output q;
   input  clk, enable, reset, d;
   reg	  q;

   /*
    *  This next block checks for timing violations of the 
    *  flip-flop's setup and hold times.
    */
   
   specify
      $setup(d, posedge clk, `setup_time);
      $hold(posedge clk, d, `hold_time);
   endspecify

   /*
    *  This is the actual code for the E-type.
    */
   
   always @(posedge clk)
      if (reset == 1)
	 #5 q = 0;
      else if (enable == 1)
	 #6 q = d;
   
endmodule /* effr */

module effs(q, clk, enable, set, d);

   /*
    *  Another behavioural model of an E-type, this time with
    *  a set line, and delays on the outputs.  The same timing
    *  checks as before are implemented here, too.
    */
   
   output q;
   input  clk, enable, set, d;
   reg	  q;
   
   specify
      $setup(d, posedge clk, `setup_time);
      $hold(posedge clk, d, `hold_time);
   endspecify
   
   always @(posedge clk)
      if (set == 1)
	 #5 q = 1;
      else if (enable == 1)
	 #6 q = d;
   
endmodule /* effs */



/****************************************************************************\
 *                                                                          *
 *    Now, the more complex modules for implementing the actual solution    *
 *                                                                          *
\****************************************************************************/

module evenSlice(bus, oneOut, zeroOut, clk, init, oneIn, zeroIn);

   /*
    *  A dataflow model of one bit slice of the full moves generator.
    *  The only differences between this module and the oddSlice one
    *  are in the initialisation values.  (Note the types of the
    *  flip-flops used.)
    */
   
   inout [3:0] bus;
   output      oneOut, zeroOut;
   input       clk, init, oneIn, zeroIn;
   
   wire	       enable, tq, tqbar;
   wire [1:0]  toPeg, fromPeg, new;
   
   toggle tog (tq, tqbar, clk, oneIn, init);
   effr   to0 (toPeg[0], clk, enable, init, new[0]);
   effs   to1 (toPeg[1], clk, enable, init, new[1]);
   effs from0 (fromPeg[0], clk, enable, init, toPeg[0]);
   effr from1 (fromPeg[1], clk, enable, init, toPeg[1]);

   assign #2 oneOut  =  oneIn & tq;
   assign #2 zeroOut = zeroIn & tqbar;
   assign #2 enable  = zeroIn & tq;
      
   assign #2 new[1] = ~(toPeg[1] & fromPeg[1]);
   assign #2 new[0] = ~(toPeg[0] & fromPeg[0]);
   
   assign bus = (enable == 1) ? {fromPeg, toPeg} : 4'bz;
   
endmodule /* evenSlice */

module oddSlice(bus, oneOut, zeroOut, clk, init, oneIn, zeroIn);

   /*
    *  See the comments for the evenSlice module.
    */
   
   inout [3:0] bus;
   output      oneOut, zeroOut;
   input       clk, init, oneIn, zeroIn;
   
   wire	       enable, tq, tqbar;
   wire [1:0]  toPeg, fromPeg, new;
   
   toggle tog (tq, tqbar, clk, oneIn, init);   
   effs   to0 (toPeg[0], clk, enable, init, new[0]);
   effs   to1 (toPeg[1], clk, enable, init, new[1]);
   effs from0 (fromPeg[0], clk, enable, init, toPeg[0]);
   effr from1 (fromPeg[1], clk, enable, init, toPeg[1]);

   assign #2 oneOut  =  oneIn & tq;
   assign #2 zeroOut = zeroIn & tqbar;
   assign #2 enable  = zeroIn & tq;
   
   assign #2 new[1] = ~(toPeg[1] & fromPeg[1]);
   assign #2 new[0] = ~(toPeg[0] & fromPeg[0]);
   
   assign bus = (enable == 1) ? {fromPeg, toPeg} : 4'bz;
   
endmodule /* evenSlice */

module start_button(go, clk, press);

   /*
    *  A gate level model of the start button, with the functionality
    *  as described elsewhere.
    */

   output  go;
   input   clk, press;
   wire	   e_out, not_press;
   supply1 vdd;

   /*
    *  This block checks that the pulse with on the input line is
    *  wider than 3, otherwise it is invalid.
    */
   
   specify
      specparam X = 3;
      $width(posedge press, X);
   endspecify      
   
   effs       st_0 (e_out, clk, vdd, press, vdd);
   not #(1)    n_0 (not_press, press);
   and #(1)  a_0 (go, e_out, not_press);

endmodule /* start_button */
	   
module tower(from_peg, to_peg, done, clk, start);

   /*
    *  This is a dataflow model of the actual move generator - to 
    *  be thought of as a stack (or tower) of modules, each of which
    *  with one disk of the puzzle.
    * 
    *  It brings together all of the other modules, and presents a
    *  clean interface to the outside world, taking a 'start' signal
    *  and returning a 'done' signal, once the sequence has been 
    *  completed.
    */
   
   output [1:0]	from_peg, to_peg;
   output	done;	
   input	clk, start;
   wire [4:0]	oneOut, zeroOut;
   wire [3:0]	bus;
   wire		init;		
   supply1	vdd;

   start_button  st_0(init, clk, start);
   oddSlice  rung0 (bus, oneOut[0], zeroOut[0], clk, ~init, vdd, vdd);
   evenSlice rung1 (bus, oneOut[1], zeroOut[1], clk, ~init,
		         oneOut[0], zeroOut[0]);   
   oddSlice  rung2 (bus, oneOut[2], zeroOut[2], clk, ~init,
		         oneOut[1], zeroOut[1]);
   evenSlice rung3 (bus, oneOut[3], zeroOut[3], clk, ~init,
		         oneOut[2], zeroOut[2]);
   oddSlice  rung4 (bus, oneOut[4], zeroOut[4], clk, ~init,
		         oneOut[3], zeroOut[3]);

   assign from_peg = bus[3:2];
   assign to_peg   = bus[1:0];
   assign done     = oneOut[4];

endmodule /* tower */


/****************************************************************************\
 *                                                                          *
 *  The final stimulus module is used to check that the tower module works  *
 *  properly                                                                *
 *                                                                          *
\****************************************************************************/

module stimulus;

   /*
    *  This is a behavioural model.  It simply instantiates the tower
    *  module, provides it with inputs and monitors its outputs.
    */

   reg	      clk;
   reg        button;
   wire [1:0] from, to;
   wire	      done;
   
   tower t_0(from, to, done, clk, button);
   
   initial begin
      clk = 0;
      forever #(`clk_period / 2) clk = ~clk;
   end
   
   initial begin
      button = 0;      
      #(`clk_period*2) button = 1;
      #(`clk_period*2) button = 0;
   end

   always @(posedge clk)
      #(`clk_period - 1)
        if ( $time > `clk_period*5 && !done)
	  $display($time, "  From peg %d To peg %d", 
				  from, to);

   always @(posedge clk)
      if (done == 1) #`clk_period $stop;
   
endmodule /* stimulus */
	