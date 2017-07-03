/* Copyright (C) 2017 Asher Blum */
function InstructionsScreen() {
  this.flasher = new Flasher(2, 0.5);
  this.nsf = 20; // hack to prevent minimum text size problems
  this.font = "" + (1.1 * this.nsf) + "px Monospace";

  this.lines = [
    "INSTRUCTIONS",
    "GET ALL GEMS", 0,
    "OR", 0,
    "KILL ALL XIMBOTS",
    "MOVE WITH ARROW KEYS",
    "THROW BALL WITH SHIFT KEY"
  ];
    

  this.draw = function() {
    M.ctx.save();
    M.ctx.scale(1/this.nsf, 1/this.nsf);
    var dy = this.nsf * M.H/6;
    var y = dy/2;
    M.ctx.font = this.font;
    M.ctx.fillStyle = "#f00";
    M.ctx.textAlign = "center";

    for(var i=0; i<this.lines.length; i++) {
      if(this.lines[i] == 0) {
        y -= .5 * dy;
      }
      else {
        M.ctx.fillText(this.lines[i], this.nsf * M.W/2, y);
        y += dy;
      }
    }

    if(this.flasher.on) {
      M.ctx.fillText("PRESS ANY KEY", this.nsf * M.W/2, y);
    }
    M.ctx.restore();
  };

  this.tic = function(ms) {
    this.flasher.tic(ms);
  }
}
