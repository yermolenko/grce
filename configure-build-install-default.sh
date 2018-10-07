#!/bin/bash
#
# Script for performing out-of-source build and installation of GRCE.
#
# Copyright (C) 2009 Alexander Yermolenko <yaa.mbox@gmail.com>
#
# Copying and distribution of this file, with or without modification,
# are permitted in any medium without royalty provided the copyright
# notice and this notice are preserved.
#

mkdir _build-default
cd _build-default
  cmake ..
  make
  make install
cd ..
