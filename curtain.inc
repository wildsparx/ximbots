/* Copyright (C) 2017 Asher Blum */
function Curtain(period_ms) {
  this.mode = 0;
  this.alpha = 0;

  this.draw = function() {
    if(this.alpha == 0) {
      return;
    }
    M.ctx.save();
    M.ctx.fillStyle = "#000";
    M.ctx.globalAlpha = this.alpha;
    M.ctx.fillRect(0, 0, M.W+1, M.H);
    M.ctx.restore();
  };

  this.go_opaque = function() {
    this.alpha = 1;
    this.mode = 0;
  };

  this.go_transparent = function() {
    this.alpha = 0;
    this.mode = 0;
  };

  this.fade_to_opaque = function(ms) {
    this.fade_per_ms = (1 - this.alpha)/ms;
    this.mode = 1;
  }

  this.fade_to_transparent = function(ms) {
    this.fade_per_ms = - this.alpha/ms;
    this.mode = -1;
  }

  this.tic = function(ms) {
    if(this.mode == 0) {
      return;
    }

    this.alpha += this.fade_per_ms * ms;

    if(this.mode == 1 && this.alpha > 1) {
      this.alpha = 1;
      this.mode = 0;
    }

    if(this.mode == -1 && this.alpha < 0) {
      this.alpha = 0;
      this.mode = 0;
    }
  };
}
