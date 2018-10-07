#  CMakeLists.txt file for the basic test of GRCTK
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

SET(GRCE_CurrentTarget "grctk-test${GRCE_BINARY_SUFFIX}")

include_directories (
  ${GRCE_SOURCE_DIR}
  ${GSL_INCLUDE_DIRS}
  ${GMP_INCLUDE_DIR}
  ${GMPXX_INCLUDE_DIR}
  )

link_directories (${GRCE_BINARY_DIR})

add_executable (${GRCE_CurrentTarget} main.cxx)

target_link_libraries (${GRCE_CurrentTarget}
  grctk
  yaatk
  ${YAATK_COMPRESSION_LIBRARIES}
  ${GSL_LIBRARIES}
  ${GMPXX_LIBRARIES}
)

IF(CMAKE_COMPILER_IS_GNUCXX)
  IF(WIN32)
    SET_TARGET_PROPERTIES(${GRCE_CurrentTarget} PROPERTIES LINK_FLAGS "-static")
  ENDIF(WIN32)
ENDIF(CMAKE_COMPILER_IS_GNUCXX)

install(TARGETS ${GRCE_CurrentTarget}
            RUNTIME DESTINATION bin
            LIBRARY DESTINATION lib
            ARCHIVE DESTINATION lib)
