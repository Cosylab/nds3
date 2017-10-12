cmake_minimum_required(VERSION 2.6)

project (nds3tests)

# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
                 ${CMAKE_BINARY_DIR}/googletest-build)

# Set compiler flags
#-------------------
set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++0x -Wall -Wextra -pedantic -pthread --coverage" )
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 --coverage")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O3")
endif()

# Set pre-processor definitions
#------------------------------
add_definitions(-DNDS3_DLL)

# Specify include and source files
#---------------------------------
include_directories(
	${ADDITIONAL_INCLUDE}
	${CMAKE_CURRENT_SOURCE_DIR}/../include
	${CMAKE_CURRENT_SOURCE_DIR}/../../include
        ${gtest_SOURCE_DIR}/include
	)

file(GLOB nds_sources "${CMAKE_CURRENT_SOURCE_DIR}/../src/*.cpp")

# Add the source files to the nds3 library
#-----------------------------------------
add_executable(nds3tests ${nds_sources})

# Add dependencies to the nds3 library
#-------------------------------------
find_library(nds3_library NAMES nds3 PATHS ${LIBRARY_LOCATION})
target_link_libraries(nds3tests ${nds3_library} gtest pthread gcov)


