#  GNU Multiple Precision Arithmetic Library detection module for CMake
#
#  Copyright (C) 2012, 2018 Alexander Yermolenko <yaa.mbox@gmail.com>
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

if (NOT GMP_INCLUDE_DIR)
  find_path(GMP_INCLUDE_DIR gmp.h
    /usr/openwin/share/include
    )
endif (NOT GMP_INCLUDE_DIR)

if (NOT GMPXX_INCLUDE_DIR)
  find_path(GMPXX_INCLUDE_DIR gmpxx.h
    /usr/openwin/share/include
    )
endif (NOT GMPXX_INCLUDE_DIR)

if (NOT GMP_LIBRARY)
  find_library(GMP_LIBRARY gmp
    /usr/openwin/lib
    )
endif (NOT GMP_LIBRARY)

if (NOT GMPXX_LIBRARY)
  find_library(GMPXX_LIBRARY gmpxx
    /usr/openwin/lib
    )
endif (NOT GMPXX_LIBRARY)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GMP DEFAULT_MSG GMP_LIBRARY GMPXX_LIBRARY GMP_INCLUDE_DIR GMPXX_INCLUDE_DIR)

set(GMP_LIBRARIES "${GMP_LIBRARY}")
set(GMPXX_LIBRARIES "${GMPXX_LIBRARY}" "${GMP_LIBRARY}" "${GMPXX_LIBRARY}" "${GMP_LIBRARY}")

mark_as_advanced(GMP_INCLUDE_DIR GMPXX_INCLUDE_DIR GMP_LIBRARY GMPXX_LIBRARY GMP_LIBRARIES GMPXX_LIBRARIES)
