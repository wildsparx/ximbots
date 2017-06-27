/* Copyright (C) 2017 Asher Blum */
function Level(lev_no) {
  this.lev_no = lev_no;
  this.dat = M.levels[lev_no];
  this.moder = new TimedModer(this, [
    ['READY',      3000],
    ['PLAY',          0],
    ['DIE',        1200],
    ['FADEUP',     1000],
    ['CELEBRATE',  3000],
    ['DONE',          0]
  ]);

  this.brite_col_no = 0;
  this.brite_col_speed = M.W/1000;
  this.brite_col_max = M.W;
  this.artifacts = [];
  this.gemsLeft = 0;
  this.monsters = [];
  this.monstersLeft = 0;
  this.artifactGrid = [];
  this.GEM_POINTS = 500;
  this.celeb_flasher = null;
  this.dbox = new DBox("GET READY!");


  this.load = function() {
    this.artifactGrid = [];
    for(var i=0; i<M.H; i++) {
      this.artifactGrid.push(new Array(M.W));
    }

    var rows = this.dat.rows;
    for(var i=0; i<M.H; i++) {
      for(var j=0; j<M.W; j++) {
        this.set_artifact_grid(j, i, -1); // empty
        var c = rows[i][j];
        switch(c) {
          case 'P':
            this.player = new Player(j, i);
            this.player.startX = j;
            this.player.startY = i;
            this.ball = new Ball(j, i);
            break;
          case '*': // Gem
            //wdebug("Got gem at " + i + " " + j);
            var a = new Gem(j, i);
            this.artifacts.push(a);
            this.set_artifact_grid(j, i, this.artifacts.length-1);
            this.gemsLeft ++;
            break;
          case 'M': // Monster generator
            var a = new MonsterGenerator(j, i);
            for(var mi=0; mi<this.dat.nMonsters; mi++) {
              var power = (mi % 2 == 1) ? 9 : 1;
              var dormMs = 1700.0 * mi;
              var m = new Monster(j, i, power, this.dat.baseMonsterSpeed + this.dat.monsterSpeedInc * mi, dormMs);
              m.hide();
              this.monsters.push(m);
              this.monstersLeft ++;
            }
            this.monsterGenerator = a;
            break;
          case 'x': // wall
            break;
          case ' ': // open
            break;
        }
      }
    }
  };

  this.set_artifact_grid = function(x, y, n) {
    this.artifactGrid[y][x] = n;
  };

  this.getArtifactGrid = function(x, y) {
    return this.artifactGrid[y][x];
  };

  // draw WxH - caller should prescale ...
  this.drawSquare = function(color0, color1, style, x, y) {
    switch(style) {
      case 0: // solid
        M.ctx.fillStyle = color0;
        M.ctx.beginPath();
        M.ctx.rect(x, y, 1, 1);
        M.ctx.fill();
        // no break, fallthru:
      case 1: // with core
        M.ctx.fillStyle = color0;
        M.ctx.beginPath();
        var k = 0.1;
        M.ctx.rect(x+k, y+k, 1, 1);
        M.ctx.fill();
        break;
      case 2: // gradient
        break; // fixme
      case 3: // diag stripe
        M.ctx.fillStyle = color0;
        M.ctx.beginPath();
        M.ctx.rect(x, y, 1, 1);
        M.ctx.fill();

        var k = 1/3;
        M.ctx.fillStyle = color1;
        M.ctx.beginPath();
        M.ctx.moveTo(x, y);
        M.ctx.lineTo(x+k, y);
        M.ctx.lineTo(x+1, y+1-k);
        M.ctx.lineTo(x+1, y+1);
        M.ctx.lineTo(x, y+k);
        M.ctx.fill();
        break;

      default: // big red X
        M.ctx.lineWidth = 0.2;
        M.ctx.strokeStyle = '#f00';
        M.ctx.moveTo(x, y);
        M.ctx.lineTo(x+1, y+1);
        M.ctx.stroke();
        M.ctx.moveTo(x+1, y);
        M.ctx.lineTo(x, y+1);
        M.ctx.lineTo(x+1, y+1);
        M.ctx.stroke();
    }

  };

  this.on_player_enter_tile = function(coords) {
    var n = this.getArtifactGrid(coords[0], coords[1]);
    if(n < 0) {
      return;
    }
    this.artifacts[n].on_player_contact();
    //wdebug("opet: " + n);
  };

  this.on_player_got_gem = function() {
    this.gemsLeft --;
    M.score.add(this.GEM_POINTS);
    M.sounds.gem.play();
    if(this.gemsLeft < 1) {
      this.start_celebrate();
    }
  };

  this.draw_self = function() {
    var rows = this.dat.rows;
    for(var i=0; i<M.H; i++) {
      var row = rows[i];
      for(var j=0; j<M.W; j++) {
        var c = row[j];
        switch(c) {
          case 'x': // wall
            this.drawSquare(this.dat.wallColor, this.dat.auxColor, this.dat.wallStyle, j, i);
            break;
          default: // space etc.
            this.drawSquare(this.dat.floorColor, this.dat.auxColor, this.dat.floorStyle, j, i);
            break;
        }
      }
    }
  };

  this.x_to_brightness = function(x) {
    var k = 3;
    var dx = Math.abs(x - this.brite_col_no);
    if(dx > k) {
      return 0;
    }
    return (k-dx)/k;
  };
  
  this.draw = function() {
    if(this.moder.mode == this.moder.modes.CELEBRATE && !this.celeb_flasher.on) {
      return;
    }
    this.draw_self();
    //wdebug("drawing artifacts: " + this.artifacts.length);
    for(var i=0; i<this.artifacts.length; i++) {
      this.artifacts[i].draw();
    }
    for(var i=0; i<this.monsters.length; i++) {
      this.monsters[i].draw();
    }
    this.monsterGenerator.draw();
    this.player.draw();
    this.ball.draw();
    if(this.moder.mode == this.moder.modes.DIE) {
      this.curtain.draw();
    }
    if(this.moder.mode == this.moder.modes.READY) {
      this.dbox.draw();
    }
  };

   /**  Given a proposed ULC coord pair for a player/enemy
    **  tell if the resulting tile is blocked
    **  coords in pixels
    **/

  this.is_blocked = function(x, y) {
    if(x < 0) {
        return true;
    }
    if(y < 0) {
        return true;
    }
    if(x > M.W-1) {
        return true;
    }
    if(y > M.H-1) {
        return true;
    }
    if(this.dat.rows[y][x] == "x") {
      return true;
    }
    return false;
};

  this.on_mode_timeout = function(mode) {
    switch(mode) { // which OLD mode
      case this.moder.modes.DIE:
        M.on_player_death();
        break;
      case this.moder.modes.FADEUP:
        this.moder.set(this.moder.modes.PLAY);
        break;
      case this.moder.modes.PLAY:
        this.moder.set(this.moder.modes.DIE);
        break;
      case this.moder.modes.CELEBRATE:
        this.moder.set(this.moder.modes.DONE);
        this.celeb_flasher = null;
        M.on_level_done();
        break;
      case this.moder.modes.READY:
        M.on_level_finish_ready();
        this.moder.set(this.moder.modes.PLAY);
        break;
    }
  };

  this.tic = function(ms) {
    this.moder.tic(ms);
    this.brite_col_no += this.brite_col_speed * ms;
    if(this.brite_col_no > this.brite_col_max) {
      this.brite_col_no = 0;
    }

    switch(this.moder.mode) {
      case this.moder.modes.PLAY:
        for(var i=0; i<this.monsters.length; i++) {
          this.monsters[i].tic(ms);
        }
        this.player.tic(ms);
        this.ball.tic(ms);
        break;
      case this.moder.modes.CELEBRATE:
        this.celeb_flasher.tic(ms);
        break;
      case this.moder.modes.READY:
        this.dbox.tic(ms);
        break;
      case this.moder.modes.DIE:
        this.curtain.tic(ms);
        break;
    }
  };

  this.on_monster_ball_contact = function() {
    //wdebug("ombc: " + this.monstersLeft);
    this.monstersLeft --;
    if(this.monstersLeft == 0) {
      this.start_celebrate();
    }
    M.sounds.mon.play();
    this.ball.on_monster_ball_contact();
  };

  this.on_monster_player_contact = function() {
    M.sounds.die.play();
    this.moder.set(this.moder.modes.DIE);
    this.curtain = new Curtain();
    this.curtain.go_transparent();
    this.curtain.fade_to_opaque(1000);
    this.player.on_monster_player_contact(); 
  };

  this.start_celebrate = function() {
    this.moder.set(this.moder.modes.CELEBRATE);
    this.celeb_flasher = new Flasher(2, 0.5);
    M.sounds.cel.play();
    //wdebug("celebrate");
  };
}
