/* Copyright (C) 2017 Asher Blum */
function Score() {
  this.score = 0;
  this.nsf = 20;
  this.font = "" + (0.8 * this.nsf) + "px Monospace";

  this.add = function(v) {
    this.score += v;
  };

  this.reset = function() {
    this.score = 0;
  };

  this.draw = function() {
    M.ctx.save();
    M.ctx.scale(1/this.nsf, 1/this.nsf);
    M.ctx.font = this.font;
    M.ctx.fillStyle = "#f00";
    M.ctx.textAlign = "right";
    M.ctx.textBaseline="middle";
    M.ctx.fillText("" + this.score, M.W*this.nsf, -.55*this.nsf);
    M.ctx.restore();
  };
}
