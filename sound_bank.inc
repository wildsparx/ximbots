/* Copyright (C) 2017 Asher Blum */
function SoundBank() {
  for(var i=0; i<M.sound_names.length; i++) {
    this[M.sound_names[i]] = new Audio("sound/" + M.sound_names[i] + ".wav");
  }
  this.stop_all = function() {
    for(var i=0; i<M.sound_names.length; i++) {
      this[M.sound_names[i]].pause();
      this[M.sound_names[i]].currentTime = 0;
    }
  };
}
