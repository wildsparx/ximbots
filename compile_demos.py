# Copyright (C) 2017 Asher Blum
import json
import re
import sys

#1.html:1454 cmd: 2032,37,299
  
def filename_to_name(fn):
  return fn.split('.')[0]

def filename_to_pairs(fn):
  nums = []
  for line in open(fn):
    m = re.match(r'\w+\.html:\d+ cmd: (\d.*)', line)
    if m:
      els = m.group(1).split(',')
      nums.extend(int(e) for e in els)
  res = []
  for i in range(0, len(nums), 3):
    res.append(nums[i:i+3])
  return res

def rm_dups(pairs):
  return pairs
  ox = -1
  oy = -1
  res = []
  for x, y, code in pairs:
    if x == ox and y == oy:
      res.pop() # override
    res.append((x, y, code))
    ox = x
    oy = y
  return res

def f2(fn):
  pairs = filename_to_pairs(fn)
  pairs = rm_dups(pairs)

def filenames_to_dict(filenames):
  res = {}
  for fn in filenames:
    name = filename_to_name(fn)
    res[name] = rm_dups(filename_to_pairs(fn))
  return res

def f1(filenames):
  d = filenames_to_dict(filenames)
  js = "M.demos = %s;" % json.dumps(d, indent=4)
  print "// autogenerated"
  print js
    
  

if __name__ == '__main__':
  f1(sys.argv[1:])
