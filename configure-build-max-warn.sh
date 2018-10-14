#!/bin/bash
#
# Script for performing out-of-source build of GRCE with maximum
# number of GCC warnings turned on.
#
# Copyright (C) 2009, 2018 Alexander Yermolenko <yaa.mbox@gmail.com>
#
# Copying and distribution of this file, with or without modification,
# are permitted in any medium without royalty provided the copyright
# notice and this notice are preserved.
#

mkdir _build-max-warn
cd _build-max-warn
  CXXFLAGS="-pedantic -W -Wall -O3" cmake ..
  make
#  make install
cd ..
