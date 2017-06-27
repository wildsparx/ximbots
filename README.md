# ximbots
Fast-paced post-Pacman arcade game. Runs in browser (at least Chrome); uses javascript.

Play it at http://wildsparx.com/ximbots/

## To Build and Play
```
make
```
point browser at /path/to/1.html

## Build Dependencies

Build assumes linux. Could probably work elsewhere with coaxing.
  * python2
  * perl
  * synthem80
  * sox

## Hacking

### Time and Animation

Objects that care about time implement a tic(ms) event which updates internal state, but doesn't draw. Drawable objects implement draw(). Objects with multiple modes have a TimedModer state machine.

### Geometry

The screen is composed of tiles, M.W x M.H. The pixel-size of tiles is auto-adjusted on screen resize.

### Directions

Directions are encoded as follows:

```
            KEY
    DIR     CODE        DIR
    --------------------------
    L       37          2
    R       30          0
    U       38          1
    D       40          3
    STOP                4

    * (dir^2) = reverse dir
    * bit 1 = axis
    * bit 2 = sign
    * bit 3 = stop
```
### Sprites

Ximbots and player each have a carriage, responsible for motion. The carriage only allows travel along open paths. A carriage has integer tile coords and a float z which defines how far into the current tile the carriage has travelled. Based on z, the carraige updates floating point pixel_coords - which are not really in pixels but in tile units.

### Level Data
    * Each level is stored in *.lev
    * mk_levels.pl reads *.lev, generates LevelData.hx
    * variables in *.lev are optional - defaults apply
    * valid characters in *.lev:
        * P = player start
        * M = monster generator
        * * = Gem
        * x = wall
        * ' ' or '.' = empty space

Copyright (C) 2017 Asher Blum <asher@wildsparx.com>
