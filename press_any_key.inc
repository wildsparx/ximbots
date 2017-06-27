/* Copyright (C) 2017 Asher Blum */
/* for use in demo mode */
function PressAny(y, bgcolor) {
  this.y = y;
  this.bgcolor = bgcolor;
  this.flasher = new Flasher(2, 0.5);

  this.draw = function() {
    M.ctx.font = "0.9px Monospace";
    M.ctx.fillStyle = "#f00";
    M.ctx.textAlign = "center";
    M.ctx.textBaseline="middle";

    if(this.flasher.on) {
      M.ctx.fillStyle = this.bgcolor;
      w = 6;
      M.ctx.fillRect(M.W/2-6, this.y-.5, 2*w, 1);
      M.ctx.fillStyle = "#f00";
      M.ctx.fillText("PRESS ANY KEY", M.W/2, this.y);
    }
  };

  this.tic = function(ms) {
    this.flasher.tic(ms);
  }
}
