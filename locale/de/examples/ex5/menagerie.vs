//
// Menagerie Initialisierungsskript
//
initial
  begin
    $readmemh("menagerie.mem");	// Lade den Speicherauszug aus Datei.
    _reset = 1'b0;		// Setze Resetleitung
    repeat (3)			// Rücke drei Taktzyklen vor
      @ (posedge clk);
    # 10;			// Rücke 10 Schritte nach der pos. Taktflanke vor
    _reset = 1'b1;		// Setze Resetleitung zurück
  end
