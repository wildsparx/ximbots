/* Copyright (C) 2017 Asher Blum */
function MonsterGenerator(x, y) {
  this.x = x;
  this.y = y;

  this.draw = function() {
    var k = 0.5;
    var j = 0.75 * k;
    M.ctx.save();
    M.ctx.translate(this.x, this.y);
    //wdebug("mg draw " + this.x + "  " + this.y);
    M.ctx.beginPath();
    M.ctx.moveTo(k-j, k+k);
    M.ctx.lineTo(k-j, k);
    M.ctx.lineTo(k-k, k);
    M.ctx.lineTo(k, 0); // peak
    M.ctx.lineTo(k+k, k); // rh corner
    M.ctx.lineTo(k+j, k); // rh corner
    M.ctx.lineTo(k+j, k+k); // rh corner
    M.ctx.fillStyle = "#aaa";
    M.ctx.fill();
    M.ctx.restore();
  };

  this.tic = function(ms) {
  };

}
