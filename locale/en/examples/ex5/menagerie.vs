//
// Menagerie Initialization Script
//
initial
  begin
    $readmemh("menagerie.mem");	// Load the memory file.
    _reset = 1'b0;		// Assert reset line
    repeat (3)			// Advance three clock steps
      @ (posedge clk);
    # 10;			// Advance 10 steps past positive clock edge
    _reset = 1'b1;		// Unassert reset line
  end
