/* Copyright (C) 2017 Asher Blum */
function CommandPlayer(events, dest) {
  this.events = events;
  this.dest = dest;
  this.i = 0;
  this.done = 0;

  this.on_player_midsquare = function(x, y) {
    if(this.i >= this.events.length) {
      this.done = 1;
    }
    if(this.done) {
      return;
    }
    // can be >1 event ...
    if(this.events[this.i][0] == x && this.events[this.i][1] == y) {
      this.dest.on_key_down({keyCode: this.events[this.i][2]});
      console.log("command_player: " + this.events[this.i]);
      this.i ++;
    }
  };
}
