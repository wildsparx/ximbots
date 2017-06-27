/* Copyright (C) 2017 Asher Blum */
/* Enable a highlight that scans across the screen
   so gems can glitter, similar to old pallette scrolling
   techniques */

function TwinkleScanner() {
  this.brite_col_no = 0;
  this.brite_col_speed = M.W/1000;
  this.brite_col_max = M.W;

  this.x_to_brightness = function(x) {
    var k = 3;
    var dx = Math.abs(x - this.brite_col_no);
    if(dx > k) {
      return 0;
    }
    return (k-dx)/k;
  };

  this.tic = function(ms) {
    this.brite_col_no += this.brite_col_speed * ms;
    if(this.brite_col_no > this.brite_col_max) {
      this.brite_col_no = 0;
    }
  };
}
