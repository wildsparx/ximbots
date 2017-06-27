/* Copyright (C) 2017 Asher Blum */
function Flasher(freq, duty_cycle) {
  this.on = 1; /* public */
  this.t = 0;
  this.freq = freq;
  this.dc = duty_cycle;
  var period = 1000 / freq;
  this.periods = [ (1-this.dc)*period, this.dc*period ];

  // Return 1/-1 if state changed; else 0
  this.tic = function(ms) {
    this.t += ms;
    if(this.t > this.periods[this.on]) {
      this.t -= this.periods[this.on];
      this.on ^= 1;
      return this.on ? 1 : -1;
    }
  };
}
