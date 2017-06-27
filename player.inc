/* Copyright (C) 2017 Asher Blum */
function Player(x, y) {
  this.c = new Carriage(this, x, y);
  this.c.restart();
  this.c.wdir = 0;
  this.last_good_dir = 0;

  this.draw = function() {
    M.ctx.save();
    this.c.translate();
    this.orient_to_dir();
    M.ctx.beginPath();
    M.ctx.moveTo(-.5, .5); // llc
    M.ctx.lineTo(.5, .5); // lrc
    M.ctx.lineTo(0, -.5); // top
    M.ctx.fillStyle = "#0f0";
    M.ctx.fill();
    M.ctx.restore();
  };

  this.orient_to_dir = function() {
    var a = Math.PI/2;
    switch(this.last_good_dir) {
      case 2: // L
        M.ctx.rotate(-a);
        break;
      case 0: // R
        M.ctx.rotate(a);
        break;
      case 3:
        M.ctx.rotate(2*a);
        break;
    }
  };

  /* from carriage - we just entered a new tile */
  this.on_enter_tile = function(coords) {
    M.level.on_player_enter_tile(coords);
  };

  this.on_monster_player_contact = function() {
  };

  this.compute_want_dir = function(stuck) {
    /* nothing - we're driven by keypress or demo */
  };

  this.tic = function(ms) {
    this.c.tic(ms);
    if(this.c.is_midsquare || this.c.dir == 4) { // midsquare or stuck
      var t = this.c._t;
      M.on_player_midsquare(t[0], t[1]);
    }
    if(this.c.dir != 4) {
      this.last_good_dir = this.c.dir;
    }
  };

  this.on_key_down = function(e) {
    switch(e.keyCode) {
      case 38: // up
        this.c.wdir = 1;
        break;
      case 40: // down
        this.c.wdir = 3;
        break;
      case 37: // left
        this.c.wdir = 2;
        break;
      case 39: // right
        this.c.wdir = 0;
        break;
      case 16: // shift = fire
        M.level.ball.try_fire();
        break;
      default:
        wdebug("player okd: " + e.keyCode);
    }
  };
}
