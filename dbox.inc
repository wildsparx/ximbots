/* Copyright (C) 2017 Asher Blum */
function DBox(msg) {
  this.msg = msg;
  this.nsf = 20; // hack to prevent minimum text size problems
  this.font = "" + (1 * this.nsf) + "px Monospace";

  this.sf = 1;
  this.ms = 0;

  this.draw = function(ms) {
    var w = 4;
    var h = 2;
    M.ctx.save();
    M.ctx.fillStyle = '#000';
    M.ctx.strokeStyle = '#f00';
    M.ctx.translate(M.W/2, M.H/2);
    M.ctx.scale(this.sf, this.sf);
    M.ctx.fillRect(-w, -h, 2*w, 2*h);
    M.ctx.lineWidth = 0.2;
    M.ctx.strokeRect(-w, -h, 2*w, 2*h);

    M.ctx.scale(1/this.nsf, 1/this.nsf);
    M.ctx.fillStyle = '#f00';
    M.ctx.textAlign = "center";
    M.ctx.textBaseline="middle";
    M.ctx.font = this.font;
    M.ctx.fillText(this.msg, 0, 0);
    M.ctx.restore();
  };

  this.tic = function(ms) {
    this.ms += ms;
    this.sf = 1 - 0.1 * Math.cos(this.ms * .006);
  };
}
