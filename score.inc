/* Copyright (C) 2017 Asher Blum */
function Score() {
  this.score = 0;

  this.add = function(v) {
    this.score += v;
  };

  this.reset = function() {
    this.score = 0;
  };

  this.draw = function() {
    M.ctx.font = "0.8px Monospace";
    M.ctx.fillStyle = "#f00";
    M.ctx.textAlign = "right";
    M.ctx.textBaseline="middle";
    M.ctx.fillText("" + this.score, M.W, -.55);
  };
}
