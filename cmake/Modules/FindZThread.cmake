#  ZThread detection module for CMake
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

if (NOT ZTHREAD_INCLUDE_DIR)
  find_path(ZTHREAD_INCLUDE_DIR zthread/Thread.h
    /usr/openwin/share/include
    )
endif (NOT ZTHREAD_INCLUDE_DIR)

if (NOT ZTHREAD_LIBRARIES)
  find_library(ZTHREAD_LIBRARIES ZThread
    /usr/openwin/lib
    )
endif (NOT ZTHREAD_LIBRARIES)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(ZThread DEFAULT_MSG ZTHREAD_LIBRARIES ZTHREAD_INCLUDE_DIR)

mark_as_advanced(ZTHREAD_INCLUDE_DIR ZTHREAD_LIBRARIES)
