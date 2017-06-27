/* Copyright (C) 2017 Asher Blum */
function SplashScreen() {
  this.gems = [];
  ngems = 14;
  this.gsf = 2; /* gem scale factor */

  this.flasher = new Flasher(2, 0.5);
  for(var i=0; i<ngems; i++) {
    var x = M.W * i / ngems / this.gsf;
    var y = (.2 * M.H + 7 * Math.random()) / this.gsf;
    this.gems.push(new Gem(x, y));
  }

  this.draw_text = function() {
    M.ctx.font = "2px Monospace";
    M.ctx.fillStyle = "#ff0";
    M.ctx.textAlign = "center";
    M.ctx.fillText("XIMBOTS", M.W/2, 3);

    M.ctx.font = "1px Monospace";
    M.ctx.fillStyle = "#f00";
    M.ctx.fillText("(C) 2017 ASHER BLUM", M.W/2, .85 * M.H);

    if(this.flasher.on) {
    M.ctx.fillText("PRESS ANY KEY", M.W/2, .95 * M.H);
    }
  };

  this.draw_monsters = function() {
    M.ctx.save();
    M.ctx.translate(M.W/4, 0.6*M.H);
    M.ctx.scale(4, 4);
    M.ctx.rotate(-.18);
    draw_monster();
    M.ctx.translate(2, 0);
    M.ctx.rotate(.29);
    draw_monster();
    M.ctx.restore();
  }

  this.draw_gems = function() {
    M.ctx.save();
    M.ctx.scale(this.gsf, this.gsf);
    for(var i=0; i<this.gems.length; i++) {
      
      this.gems[i].draw();
    }
    M.ctx.restore();
  }

  this.draw = function() {
    this.draw_gems();
    this.draw_monsters();
    this.draw_text();
  }

  this.tic = function(ms) {
    this.flasher.tic(ms);
  }
}
