/* Copyright (C) 2017 Asher Blum */
function InstructionsScreen() {
  this.flasher = new Flasher(2, 0.5);
  this.lines = [
    "INSTRUCTIONS",
    "GET ALL GEMS", 0,
    "OR", 0,
    "KILL ALL XIMBOTS",
    "MOVE WITH ARROW KEYS",
    "THROW BALL WITH SHIFT KEY"
  ];
    

  this.draw = function() {
    var dy = M.H/6;
    var y = dy/2;
    M.ctx.font = "1.1px Monospace";
    M.ctx.fillStyle = "#f00";
    M.ctx.textAlign = "center";

    for(var i=0; i<this.lines.length; i++) {
      if(this.lines[i] == 0) {
        y -= .5 * dy;
      }
      else {
        M.ctx.fillText(this.lines[i], M.W/2, y);
        y += dy;
      }
    }

    if(this.flasher.on) {
      M.ctx.fillText("PRESS ANY KEY", M.W/2, y);
    }
  };

  this.tic = function(ms) {
    this.flasher.tic(ms);
  }
}
