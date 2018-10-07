#  CMakeLists.txt file for GRCE
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

SET(GRCE_CurrentTarget "grce${GRCE_BINARY_SUFFIX}")

include_directories (
  ${GRCE_SOURCE_DIR}
  ${GSL_INCLUDE_DIRS}
  ${GMP_INCLUDE_DIR}
  ${GMPXX_INCLUDE_DIR}
  ${ZTHREAD_INCLUDE_DIR}
  ${FLTK_INCLUDE_DIR}
  )

link_directories (${GRCE_BINARY_DIR})

add_executable (${GRCE_CurrentTarget}
  GRCE.cxx
  main.cxx
  VertexWidget.cxx
  EdgeWidget.cxx
  Color.cxx
  propedit/EPropEdit.cxx
  propedit/VPropEdit.cxx
  propedit/PropEdit.cxx
  DocControl.cxx
  Wizard.cxx
  ParamsDialog.cxx
  bmpImage.cxx
  DocToolbox.cxx
  MainWindow.cxx
  GraphEdit.cxx
  Attributes.cxx
  DocWindow.cxx
  EnergyChart.cxx
  EnergySpectrum.cxx
  SelectOneDlg.cxx
  LogExecutor.cxx
  Runnable.cxx
  VisBox.cxx
  VisWindow.cxx
  algo/color/ResetColors.cxx
  algo/color/Attr2Color.cxx
  Saver.cxx
  Loader.cxx
)

IF(WIN32)
  target_link_libraries (${GRCE_CurrentTarget}
    grctk
    yaatk
    ${FLTK_LIBRARIES}
    ${OPENGL_LIBRARIES}
    ${YAATK_COMPRESSION_LIBRARIES}
    ${GSL_LIBRARIES}
    ${GMPXX_LIBRARIES}
    ${ZTHREAD_LIBRARIES}
    ole32 uuid comctl32 wsock32 gdi32)
ELSE(WIN32)
  target_link_libraries (${GRCE_CurrentTarget}
    grctk
    yaatk
    ${FLTK_LIBRARIES}
    ${OPENGL_LIBRARIES}
    ${YAATK_COMPRESSION_LIBRARIES}
    ${GSL_LIBRARIES}
    ${GMPXX_LIBRARIES}
    ${ZTHREAD_LIBRARIES})
ENDIF(WIN32)

IF(CMAKE_COMPILER_IS_GNUCXX)
  IF(WIN32)
    SET_TARGET_PROPERTIES(${GRCE_CurrentTarget} PROPERTIES LINK_FLAGS "-static")
  ENDIF(WIN32)
ENDIF(CMAKE_COMPILER_IS_GNUCXX)

IF(PNG_FOUND)
  MESSAGE(STATUS "libpng found. Using libpng in grce.")
  target_link_libraries (${GRCE_CurrentTarget} ${PNG_LIBRARIES})
  SET_TARGET_PROPERTIES(${GRCE_CurrentTarget} PROPERTIES COMPILE_FLAGS "-DGRCE_PNG")
ELSE(PNG_FOUND)
  MESSAGE(STATUS "libpng not found. grce will write BMP files instead of PNG.")
ENDIF(PNG_FOUND)

install(TARGETS ${GRCE_CurrentTarget}
            RUNTIME DESTINATION bin
            LIBRARY DESTINATION lib
            ARCHIVE DESTINATION lib)

