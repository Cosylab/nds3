cmake_minimum_required(VERSION 2.6)

project (nds3tests)

# Set compiler flags
#-------------------
set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++0x -Wall -Wextra -pedantic -pthread --coverage" )

# Set pre-processor definitions
#------------------------------
add_definitions(-DNDS3_DLL)

# Specify include and source files
#---------------------------------
include_directories(
	${ADDITIONAL_INCLUDE}
	${CMAKE_CURRENT_SOURCE_DIR}/../include
	${CMAKE_CURRENT_SOURCE_DIR}/../../include
	)

file(GLOB nds_sources "${CMAKE_CURRENT_SOURCE_DIR}/../src/*.cpp")

# Add the source files to the nds3 library
#-----------------------------------------
add_executable(nds3tests ${nds_sources})

# Add dependencies to the nds3 library
#-------------------------------------
find_library(nds3_library NAMES nds3 PATHS ${LIBRARY_LOCATION})
target_link_libraries(nds3tests ${nds3_library} ${CMAKE_CURRENT_BINARY_DIR}/libgtest.a pthread gcov)


