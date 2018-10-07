#!/bin/bash
#
# Script for performing out-of-source build and installation of GRCE.
# (Passing heavy optimize options to compiler, see top-level
# CMakeLists.txt for details).
#
# Copyright (C) 2009, 2015 Alexander Yermolenko <yaa.mbox@gmail.com>
#
# Copying and distribution of this file, with or without modification,
# are permitted in any medium without royalty provided the copyright
# notice and this notice are preserved.
#

mkdir _build-heavy-optimize
cd _build-heavy-optimize
  cmake -D GRCE_HEAVY_OPTIMIZE=1 ..
  make
  make install
cd ..

