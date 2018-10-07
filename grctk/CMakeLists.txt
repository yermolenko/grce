#  CMakeLists.txt file for GRCTK, the Graph Research and Computing Toolkit.
#
#  Copyright (C) 2007, 2008, 2009, 2010, 2011, 2015, 2016, 2017, 2018
#  Alexander Yermolenko <yaa.mbox@gmail.com>
#
#  This file is part of GRCE, the Graph Research and Computing Environment.
#
#  GRCE is free software: you can redistribute it and/or modify it
#  under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  GRCE is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with GRCE.  If not, see <http://www.gnu.org/licenses/>.
#

include_directories (
  ${GRCE_SOURCE_DIR}
  ${GSL_INCLUDE_DIRS}
  ${GMP_INCLUDE_DIR}
  ${GMPXX_INCLUDE_DIR}
  ${ZTHREAD_INCLUDE_DIR}
  )

add_library (grctk
  Universe.cxx
  AdjMatrix.cxx
  algo/StdLog.cxx
  algo/StringedStdLog.cxx
  algo/StringLog.cxx
  algo/generation/GenComplete.cxx
  algo/generation/GenRandom.cxx
  algo/products/Product.cxx
  algo/products/CartesianProduct.cxx
  algo/products/TensorProduct.cxx
  algo/products/LexicographicalProduct.cxx
  algo/products/StrongProduct.cxx
  algo/isomorphism/CMR.cxx
  algo/drawing/random/RandomizePositions.cxx
  algo/drawing/transform/MovePositions.cxx
  algo/drawing/GraphMultiRep.cxx
  algo/drawing/pairpot/PairPotBase.cxx
  algo/drawing/pairpot/PairPot.cxx
  algo/drawing/intersections/OptiIntersect.cxx
  algo/connectivity/ConComp.cxx
  algo/orbits/FindOrbitsSubgraphIso.cxx
  algo/orbits/FindOrbitsVPerms.cxx
  algo/orbits/FindOrbitsEdgeTensions.cxx
  algo/orbits/CmpSubgraphIsoAndEdgeTensions.cxx
  algo/formats/Environment.cxx
  algo/formats/LinkTable.cxx
  algo/formats/BinCode.cxx
  algo/formats/BinCodeFile.cxx
  algo/properties/BasicProperties.cxx
  algo/NullLog.cxx
  )

install(TARGETS grctk
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)

install(DIRECTORY ./ DESTINATION include/grctk FILES_MATCHING PATTERN "*.h")
install(DIRECTORY ./ DESTINATION include/grctk FILES_MATCHING PATTERN "*.hpp")
install(DIRECTORY ./ DESTINATION include/grctk FILES_MATCHING PATTERN "*.inc")
