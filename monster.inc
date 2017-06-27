/* Copyright (C) 2017 Asher Blum */
/* static so it can be used in both game and splash screen: */
function draw_monster() {
  M.ctx.beginPath();
  M.ctx.moveTo(-.5, .5); // llc
  M.ctx.lineTo(-.25, -.5); // ulc
  M.ctx.lineTo(.25, -.5); // urc
  M.ctx.lineTo(.5, .5); // lrc
  M.ctx.fillStyle = "#f00";
  M.ctx.fill();

  // eye
  var k = 0.15;
  M.ctx.beginPath();
  M.ctx.moveTo(-1.5 * k, -k);
  M.ctx.lineTo(-1.5 * k, -2 * k);
  M.ctx.lineTo(-.5 * k, -2 * k);
  M.ctx.lineTo(-.5 * k, -1 * k);
  M.ctx.fillStyle = "#000";
  M.ctx.fill();
}

function Monster(x, y, power, base_speed, dormant_ms) {
  this.c = new Carriage(this, x, y);
  this.c.restart();
  this.c.speed = base_speed;
  this.chase_point = [0, 0];
  this.power = power;
  this.base_speed = base_speed;
  this.dormant_ms = dormant_ms;
  this.pointing_right = 0;
  this.death_ang = 0;
  this.death_scale = 1;
  
  this.moder = new TimedModer(this, [
    ['DORMANT',   dormant_ms+1],
    ['NORMAL',    0],
    ['DYING',     500],
    ['DEAD',      0]
  ]);

  this.dist_to_chase_point = function() {
    var dx = this.c._t[0] - this.chase_point[0];
    var dy = this.c._t[1] - this.chase_point[1];
    return Math.sqrt(dx*dx + dy*dy);
  };

  /* called by carriage */
  this.compute_want_dir = function(stuck) {
    var mag = 4 * (this.power-5); // -5 to 4 // seems weak
    M.level.player.c.project_dir(mag, this.chase_point); // chase a point near the player
    //wdebug("monster: mag=" + mag);
    var min_pd = 1000000;
    var min_dir = this.c.dir;
    for(var dir = 0; dir < 4; dir ++) {
      this.c.moveT(dir);
      var blocked = M.level.is_blocked(this.c._t[0], this.c._t[1]);
      var npd = this.dist_to_chase_point();
      this.c.moveT(dir ^ 2);
      if(npd < min_pd && !blocked && dir != (this.c.dir^2)) {
        min_pd = npd;
        min_dir = dir;
      }
    }
    //wdebug("cwd: dir=" + this.c.dir + "  inverse=" + (this.c.dir^2) + "  new=" + min_dir);
    this.c.wdir = min_dir;
  };

  /* called by carriage */
  this.on_enter_tile = function(coords) {
  };

  this.draw = function() {
    M.ctx.save();
    this.c.translate();
    if(this.moder.mode == this.moder.modes.DYING) {
      M.ctx.scale(this.death_scale, this.death_scale);
      M.ctx.rotate(this.death_ang);
      M.ctx.globalAlpha = 0.4;
    }
    switch(this.moder.mode) {
      case this.moder.modes.NORMAL:
      case this.moder.modes.DYING:
        if(this.point_right) {
          M.ctx.scale(-1, 1);
        }
        draw_monster();
        break;
    }
    M.ctx.restore();
  };

  this.check_ball_collision = function() {
    if(!M.level.ball.fatal) {
      return;
    }
    if(this.moder.mode != this.moder.modes.NORMAL) {
      return;
    }
    var dist = this.c.distance_to_carriage(M.level.ball.c);
    if(dist > .5) {
      return;
    }
    this.moder.set(this.moder.modes.DYING);
    M.level.on_monster_ball_contact();
  };

  this.check_player_collision = function() {
    if(this.moder.mode != this.moder.modes.NORMAL) {
      return;
    }
    var dist = this.c.distance_to_carriage(M.level.player.c);
    if(dist > .25) {
      return;
    }
    M.level.on_monster_player_contact();

  };

  this.tic = function(ms) {
    this.moder.tic(ms);
    switch(this.moder.mode) {
      case this.moder.modes.NORMAL:
        this.c.tic(ms);
        if(this.c.dir == 2) {
          this.point_right = 0;
        }
        else if(this.c.dir == 0) {
          this.point_right = 1;
        }
        this.check_ball_collision();
        this.check_player_collision();
        break;
      case this.moder.modes.DYING:
        this.death_ang += 6 * ms/1000;
        this.death_scale += M.H * ms / 500;
        break;
    }
  };

  this.on_mode_timeout = function(mode) {
    switch(mode) {
      case this.moder.modes.DORMANT:
        this.moder.set(this.moder.modes.NORMAL);
        break;
      case this.moder.modes.DYING:
        this.moder.set(this.moder.modes.DEAD);
        break;
    }
  };

  this.hide = function() {
  };

}
