# Copyright (C) 2017 Asher Blum
'''Read *.lev and write javascript to stdout'''

import copy
import json
import os
import sys
import re

# playfield dimensions:
W = 17
H = 17

DEFAULTS = {
    'floorColor'          : '220000',
    'floorStyle'          : 0,
    'wallColor'           : '000099',
    'wallStyle'           : 0,
    'auxColor'            : '0000bb',
    'baseMonsterSpeed'    : .004,
    'nMonsters'           : 4,
    'monsterSpeedInc'     : .0005,
}

def get_lev_filenames():
  return sorted([f for f in os.listdir('.') if f.endswith('.lev')], key=lambda fn: int(fn.rstrip('.lev')))

def process_val(key, val):
  '''str -> str/int/float.  int/float makes no diff since all goes to JSON ...'''
  if key.endswith('Color') and val.startswith('0x'):
    return '#' + val[2:]
  if val.isdigit():
    return int(val)
  return float(val)

def fn_to_level(fn):
  attrs = copy.copy(DEFAULTS)
  rows = []
  for line in open(fn).readlines():
    m = re.match(r'\$(\w+)\s+(\S+)', line)
    if line.startswith('#'):
      continue
    if m:
      attrs[m.group(1)] = process_val(m.group(1), m.group(2))
      continue
    line = line.rstrip("\n")
    if len(line) == W:
      rows.append(line.replace('.', ' '))
      continue
    raise ValueError("Unexpeced line in %r: %r" % (fn, line))
  if len(rows) != H:
    raise ValueError("Expected %d rows, got %d in %r" % (H, len(rows), fn))
  attrs['rows'] = rows
  return attrs

def f1():
  levels = [fn_to_level(fn) for fn in get_lev_filenames()]
  js = json.dumps(levels, indent=4).rstrip();
  print "// ===== Autogenerated by %s =====" % sys.argv[0]
  print "M.levels = %s;" % js

f1()
