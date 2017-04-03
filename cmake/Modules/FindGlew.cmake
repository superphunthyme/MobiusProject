# Copyright (C) 2007-2009 LuaDist.
# Created by Peter Kapec <kapecp@gmail.com>
# Redistribution and use of this file is allowed according to the terms of the MIT license.
# For details see the COPYRIGHT file distributed with LuaDist.
#	Note:
#		Searching headers and libraries is very simple and is NOT as powerful as scripts
#		distributed with CMake, because LuaDist defines directories to search for.
#		Everyone is encouraged to contact the author with improvements. Maybe this file
#		becomes part of CMake distribution sometimes.

# - Find glew
# Find the native glew headers and libraries.
#
#  GLEW_INCLUDE_DIRS - where to find glew.h, etc.
#  GLEW_LIBRARIES    - List of libraries when using glew.
#  GLEW_FOUND        - True if glew found.

# Look for the header file.
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} "/h/opt/glew-1.13.0/")
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} "C:/Program Files/Common Files/MSVC/glew-1.13.0")
# message("Cmake prefix path: ${CMAKE_PREFIX_PATH}")

FIND_PATH(GLEW_INCLUDE_DIR NAMES GL/glew.h)

# Look for the library.
FIND_LIBRARY(GLEW_LIBRARY NAMES glew32
  HINTS PATH_SUFFIXES Release/x64)

# Handle the QUIETLY and REQUIRED arguments and set glew_FOUND to TRUE if all listed variables are TRUE.
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GLEW DEFAULT_MSG GLEW_LIBRARY GLEW_INCLUDE_DIR)

# Copy the results to the output variables.
IF(GLEW_FOUND)
	SET(GLEW_LIBRARIES ${GLEW_LIBRARY})
	SET(GLEW_INCLUDE_DIRS ${GLEW_INCLUDE_DIR})
ELSE(GLEW_FOUND)
	SET(GLEW_LIBRARIES)
	SET(GLEW_INCLUDE_DIRS)
ENDIF(GLEW_FOUND)

MARK_AS_ADVANCED(GLEW_INCLUDE_DIRS GLEW_LIBRARIES)
