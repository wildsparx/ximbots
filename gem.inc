/* Copyright (C) 2017 Asher Blum */
function Gem(x, y) {
  this.x = x;
  this.y = y;
  this.live = 1;

  this.draw = function() {
    briteness = M.twinkle_scanner.x_to_brightness(this.x);
    if(!this.live) {
      return;
    }
    M.ctx.save();
    M.ctx.translate(this.x, this.y);
    M.ctx.beginPath();
    M.ctx.moveTo(.5, .25);
    M.ctx.lineTo(.75, .5);
    M.ctx.lineTo(.5, .75);
    M.ctx.lineTo(.25, .5);
    var pwr = Math.floor(180 + 70 * briteness);
    M.ctx.fillStyle = 'rgb(0, ' + pwr + ', 0)';
    M.ctx.fill();
    M.ctx.restore();
  };

  this.on_player_contact = function() {
    M.level.on_player_got_gem();
    M.level.set_artifact_grid(this.x, this.y, -1);
    this.live = 0;
  };

  this.tic = function(ms) {
  };

}
