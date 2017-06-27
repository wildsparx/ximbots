/* Copyright (C) 2017 Asher Blum */
/* State machine to switch modes, sometimes on a timer 
   mode_specs = ['MODE_NAME', ms], or 0 meaning forever 

   API: construct, tic(ms), set_mode(mode), ->parent.on_mode_timeout()
*/

function TimedModer(parent, mode_specs) {
  this.mode2ms = {};
  this.modes = {}; // for public reference
  for(var i=0; i<mode_specs.length; i++) {
    var label = mode_specs[i][0];
    var ms = mode_specs[i][1];
    this.mode2ms[label] = ms;
    this.modes[label] = label;
  }
  Object.freeze(this.modes);

  this._real_tic = function(ms) {
    this.mode_ms -= ms;
    if(this.mode_ms < 0) {
      parent.on_mode_timeout(this.mode);
    }
  };

  this._fake_tic = function(ms) { };

  this.set = function(mode) {
    this.mode = mode;
    this.mode_ms = this.mode2ms[mode];
    this.tic = this.mode_ms ? this._real_tic : this._fake_tic;
  };

  this.set(mode_specs[0][0]);
}
