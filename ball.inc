/* Copyright (C) 2017 Asher Blum */
function Ball(x, y) {
  this.x = x;
  this.y = y;
  this.c = new Carriage(this, x, y);
  this.bounced = 0;
  this.last_dir = 0;
  this.fatal = 0; // public indicator - ball is fatal to monsters

  this.moder = new TimedModer(this, [
    ['READY', 0],
    ['MOVING', 4000],
    ['RECHARGING', 4000]
  ]);

  this.compute_want_dir = function(stuck) {
    if(!stuck) {
      return;
    }
    this.bounced = 1;
    this.last_dir ^= 2;
    this.c.wdir = this.last_dir;
    M.sounds.bnc.play();
  };

  this.on_enter_tile = function(coords) {
  };

  this.draw = function() {
    if(this.moder.mode == this.moder.modes.RECHARGING) {
      return;
    }
    M.ctx.save();
    if(this.moder.mode == this.moder.modes.READY) {
      M.level.player.c.translate();
    }
    else { // moving
      this.c.translate();
    }
    M.ctx.beginPath();
    M.ctx.arc(0, 0, 0.25, 0, 2 * Math.PI, false);
    M.ctx.fillStyle = "#fff";
    M.ctx.fill();
    M.ctx.restore();
  };

  this.try_fire = function() {
    if(this.moder.mode != this.moder.modes.READY) {
      wdebug("ball: fire failed");
      return;
    }
    this.bounced = 0;
    this.moder.set(this.moder.modes.MOVING);
    var p = M.level.player;
    this.c.start_x = p.c._t[0];
    this.c.start_y = p.c._t[1];
    this.c.restart();
    this.c.dir = p.c.dir;
    this.last_dir = this.c.dir;
    this.c.wdir = p.c.wdir;
    this.c.speed = 2 * p.c.speed; // FIXME
    // avoid glitch - start ball at player position
    this.c.z = p.c.z;
    this.c.set_pixel_coords();
    this.fatal = 1;
    M.sounds.sht.play();
  };

  this.on_mode_timeout = function(mode) {
    switch(mode) {
      case this.moder.modes.MOVING: // independent
        this.moder.set(this.moder.modes.RECHARGING);
        this.fatal = 0;
        break;
      case this.moder.modes.RECHARGING:
        this.moder.set(this.moder.modes.READY);
        this.fatal = 0;
        break;
    }
  };

  this.tic = function(ms) {
    this.moder.tic(ms);
    switch(this.moder.mode) {
      case this.moder.modes.MOVING: // independent
        this.c.tic(ms);
        if(this.bounced && (this.c.distance_to_carriage(M.level.player.c) < 0.5)) {
          this.moder.set(this.moder.modes.READY);
          this.fatal = 0;
          return;
        }
        break;
    }
  };

  this.on_monster_ball_contact = function() {
    this.moder.set(this.moder.modes.RECHARGING);
    this.fatal = 0;
  };

}
