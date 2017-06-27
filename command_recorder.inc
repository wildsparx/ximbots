/* Copyright (C) 2017 Asher Blum */
function CommandRecorder(start_time) {
  this.events = [];
  this.last_event = null;
  this.max_events = 200; // for safety

  this.on_key = function(x, y, key_code) {
    if(this.last_event && this.last_event[0] == x && this.last_event[1] == y && this.last_event[2] == key_code) {
      return;
    }
    if(this.events.length > this.max_events.length) {
      return;
    }
    this.last_event = [x, y, key_code];
    this.events.push(this.last_event);
  };

  /* Export events in a parseable format */
  this.wlog = function() {
    console.clear();
    console.log("CommandRecorder wlog: " + this.events.length);
    var chunk_size = 10;
    for(var i=0; i<this.events.length; i += chunk_size) {
      console.log("cmd: " + this.events.slice(i, i+chunk_size));
    }
  };
}
