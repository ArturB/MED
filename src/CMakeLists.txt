cmake_minimum_required (VERSION 2.6)
project (MED)

# The version number.
set (Tutorial_VERSION_MAJOR 1)
set (Tutorial_VERSION_MINOR 0)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/MedConfig.h.in"
  "${PROJECT_BINARY_DIR}/MedConfig.h"
  )
include_directories ("${PROJECT_BINARY_DIR}")
  
option (USE_MYMATH 
        "Use tutorial provided math implementation" ON)

# add the MathFunctions library?
if (USE_MYMATH)
  include_directories ("${PROJECT_SOURCE_DIR}/MathFunctions")
  set (EXTRA_LIBS ${EXTRA_LIBS} MathFunctions)
endif (USE_MYMATH)
 
# add the executable
add_executable (MED main.cpp)
target_link_libraries (MED  ${EXTRA_LIBS})

# add the install targets
install (TARGETS MED DESTINATION bin)
install (FILES "${PROJECT_BINARY_DIR}/MedConfig.h"        
         DESTINATION include)


# build a CPack driven installer package
include (InstallRequiredSystemLibraries)
set (CPACK_RESOURCE_FILE_LICENSE  
     "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set (CPACK_PACKAGE_VERSION_MAJOR "${Tutorial_VERSION_MAJOR}")
set (CPACK_PACKAGE_VERSION_MINOR "${Tutorial_VERSION_MINOR}")
include (CPack)
		
