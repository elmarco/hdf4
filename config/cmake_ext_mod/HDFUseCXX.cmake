# This file provides functions for C++ support.
#
#-------------------------------------------------------------------------------
ENABLE_LANGUAGE (CXX)
set (HDF_PREFIX "H4")

#-------------------------------------------------------------------------------
#  Fix CXX flags if we are compiling staticly on Windows using
#  Windows_MT.cmake from config/cmake/UserMacros
#-------------------------------------------------------------------------------
if (BUILD_STATIC_CRT_LIBS)
  TARGET_STATIC_CRT_FLAGS ()
endif ()

#-----------------------------------------------------------------------------
# Configure Checks which require CXX compilation must go in here
# not in the main ConfigureChecks.cmake files, because if the user has
# no CXX compiler, problems arise.
#-----------------------------------------------------------------------------
include (CheckIncludeFileCXX)
include (TestForSTDNamespace)

# IF the c compiler found stdint, check the C++ as well. On some systems this
# file will be found by C but not C++, only do this test IF the C++ compiler
# has been initialized (e.g. the project also includes some c++)
if (${HDF_PREFIX}_HAVE_STDINT_H AND CMAKE_CXX_COMPILER_LOADED)
  CHECK_INCLUDE_FILE_CXX ("stdint.h" ${HDF_PREFIX}_HAVE_STDINT_H_CXX)
  if (NOT ${HDF_PREFIX}_HAVE_STDINT_H_CXX)
    set (${HDF_PREFIX}_HAVE_STDINT_H "" CACHE INTERNAL "Have includes HAVE_STDINT_H")
    set (USE_INCLUDES ${USE_INCLUDES} "stdint.h")
  endif ()
endif ()
