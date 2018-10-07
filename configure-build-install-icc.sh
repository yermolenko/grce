#!/bin/bash
#
# Script for performing out-of-source build and installation of GRCE.
# (Example of using Intel C++ compiler).
#
# Copyright (C) 2009, 2011 Alexander Yermolenko <yaa.mbox@gmail.com>
#
# Copying and distribution of this file, with or without modification,
# are permitted in any medium without royalty provided the copyright
# notice and this notice are preserved.
#

mkdir _build-icc
cd _build-icc
#  uncomment the next line when using icc 11.1 with gcc 4.5 or higher
#  export GXX_INCLUDE=/usr/include/c++/4.4
  source /opt/intel/Compiler/11.1/064/bin/intel64/iccvars_intel64.sh
  LANG=C CC=icc CXX=icpc cmake -D BUILD_INTELCC=1 ..
  LANG=C make
  LANG=C make install
cd ..
