# Copyright (C) 2007-2009 LuaDist.
# Created by Peter Kapec <kapecp@gmail.com>
# Redistribution and use of this file is allowed according to the terms of the MIT license.
# For details see the COPYRIGHT file distributed with LuaDist.
#	Note:
#		Searching headers and libraries is very simple and is NOT as powerful as scripts
#		distributed with CMake, because LuaDist defines directories to search for.
#		Everyone is encouraged to contact the author with improvements. Maybe this file
#		becomes part of CMake distribution sometimes.

# - Find Freeimage
# Find the native Freeimage headers and libraries.
#
#  FREEIMAGE_INCLUDE_DIRS - where to find freeimage.h, etc.
#  FREEIMAGE_LIBRARIES    - List of libraries when using Freeimage.
#  FREEIMAGE_FOUND        - True if Freeimage found.

set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} "/h/opt/FreeImage-3.17.0/")
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} "C:/Program Files/Common Files/MSVC/FreeImage")

# Look for the header file.
FIND_PATH(FREEIMAGE_INCLUDE_DIR NAMES FreeImage.h)
#  HINTS PATH_SUFFIXES FreeImage-3.17.0)

# Look for the library.
FIND_LIBRARY(FREEIMAGE_LIBRARY NAMES freeimage
    HINTS PATH_SUFFIXES x64)

# Handle the QUIETLY and REQUIRED arguments and set FREEIMAGE_FOUND to TRUE if all listed variables are TRUE.
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(FREEIMAGE DEFAULT_MSG FREEIMAGE_LIBRARY FREEIMAGE_INCLUDE_DIR)

# Copy the results to the output variables.
IF(FREEIMAGE_FOUND)
	SET(FREEIMAGE_LIBRARIES ${FREEIMAGE_LIBRARY})
	SET(FREEIMAGE_INCLUDE_DIRS ${FREEIMAGE_INCLUDE_DIR})
ELSE(FREEIMAGE_FOUND)
	SET(FREEIMAGE_LIBRARIES)
	SET(FREEIMAGE_INCLUDE_DIRS)
ENDIF(FREEIMAGE_FOUND)

MARK_AS_ADVANCED(FREEIMAGE_INCLUDE_DIRS FREEIMAGE_LIBRARIES)
