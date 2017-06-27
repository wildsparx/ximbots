/* Copyright (C) 2017 Asher Blum */
/* payload must implement:
   compute_want_dir(am_blocked)
   on_enter_tile(_t)
*/

function Carriage(payload, x, y) {
  this.payload = payload;
  this.start_x = x;
  this.start_y = y;
  this.is_midsquare = 0;
  this.z = 0;
  this.speed = 0;
  this._t = [0, 0]; /* int x, y (tile coords) */

  this.orient_to_dir = function() {} // override this

  this.translate = function() {
    M.ctx.translate(this.pixel_coords[0], this.pixel_coords[1]);
  };

  this.restart = function() {
    this._t = [this.start_x, this.start_y];
    this.pixel_coords = [this.start_x + 0.5, this.start_y + 0.5];
    this.dir = 2; // actual dir
    this.wdir = 2; // want dir
    this.speed = .009;
    this.z = 0;
    this.nmoves = 0;
    this.moves_hist = [];
  }

  this.distance_to_carriage = function(c) {
    var dx = this.pixel_coords[0] - c.pixel_coords[0];
    var dy = this.pixel_coords[1] - c.pixel_coords[1];
    return Math.sqrt(dx*dx + dy*dy);
  };

  // Project my current coordinates forward by mag * dir,
  // storing result in result.
  this.project_dir = function(mag, result) {
    result[0] = this._t[0];
    result[1] = this._t[1];
    var axis = this.dir & 1;
    var d = ((this.dir & 2) != 0) ? -1 : 1;
    if(axis == 1) { // hack
      d *= -1;
    }
    result[axis] += mag * d;
  };

  this.set_pixel_coords = function() {
    this.pixel_coords[0] = this._t[0] + 0.5;
    this.pixel_coords[1] = this._t[1] + 0.5;
    var axis = this.dir & 1;
    var d = ((this.dir & 2) != 0) ? -1 : 1;
    if(axis == 1) { // hack
      d *= -1;
    }
    this.pixel_coords[axis] += d * this.z; // hack
  };

    // move _t one click based on dir:
  this.moveT = function(dir) {
    //this.moves_hist.push(dir); // 2 2 0 2 0
    if(dir == 4) { // no-op
        return;
    }
    if((dir & 4) == 4) { // no-op
        return;
    }
    this.nmoves ++;
    var axis = dir & 1;
    var d = ((dir & 2) != 0) ? -1 : 1;
    if(axis == 1) { // hack
      d *= -1;
    }
    this._t[axis] += d;
  }


  this.tic = function(ms) {
    var old_dir = this.dir;
    if(this.dir != 4) {
      var oz = this.z;
      this.z += this.speed * ms;
      this.is_midsquare = (this.z >= .2 && oz < .8);
    }
    if(this.dir == 4) { // stuck
      this.z = 0;
      var blocked = false;
      this.payload.compute_want_dir(true);
      if(this.wdir != this.dir) { // want to start moving
          this.moveT(this.wdir); // see if it works
          blocked = M.level.is_blocked(this._t[0], this._t[1]);
          this.moveT(this.wdir ^ 2); // reverse it
      }
      if(!blocked) {
          this.dir = this.wdir;
      }
    }

    else if(this.z > 1) { // crossed threshold into next cell
      this.moveT(this.dir);
      this.payload.compute_want_dir(false);
      if(M.level.is_blocked(this._t[0], this._t[1])) {
          this.moveT(this.dir ^ 2); // reverse it
          this.dir = 4; // stop
          this.z = 0;
      }
      else { // check one block ahead
          this.moveT(this.dir);
          var blocked = M.level.is_blocked(this._t[0], this._t[1]);
          this.moveT(this.dir ^ 2); // reverse it
          if(blocked) {
              this.dir = 4; // stop
              this.z = 0;
          }
          this.payload.on_enter_tile(this._t);
      }

      if(this.z > 1) {
        this.z -= 1;
      }
      if(this.wdir !=this.dir ) { // want to change direction
        this.moveT(this.wdir); // see if it works
        if(!M.level.is_blocked(this._t[0], this._t[1])) {
            this.dir = this.wdir; 
            //moveT(_wdir ^ 2); // reverse it // apb
            //trace("set dir=" + _dir + "; z=" + _z);
        } 
        this.moveT(this.wdir ^ 2); // reverse it // apb
      }
    }
    this.set_pixel_coords();
    //wdebug("car bottom old_dir=" + old_dir + "  dir=" + this.dir + "  _t=" + this._t + "  z=" + this.z);
  };
} 
