#!/usr/bin/python2
# Copyright (C) 2017 Asher Blum

import re
import sys

def include_one(page):
  m = re.search(r'\n@include\((.+?)\)', page)
  if not m:
    return None
  fn = m.group(1)
  spage = open(fn).read()
  return page.replace('\n@include(%s)' % fn, "\n" + spage)

def include_all(page):
  while True:
    p2 = include_one(page)
    if not p2:
      return page
    page = p2

def main(src_fn, dest_fn):
  page = open(src_fn).read()
  page = include_all(page)
  open(dest_fn, 'w').write(page)

if __name__ == '__main__':
  main(sys.argv[1], sys.argv[2])
