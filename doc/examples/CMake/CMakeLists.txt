cmake_minimum_required(VERSION 2.6)

project (nds3examples)

set( VERSION "1.0.0")

# Set compiler flags
#-------------------
set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++0x -Wall -Wextra -pedantic -fPIC -pthread" )

# Set pre-processor definitions
#------------------------------
add_definitions(-DNDS3_DLL)

# Specify include and source files
#---------------------------------
include_directories(
	${ADDITIONAL_INCLUDE}
	${CMAKE_CURRENT_SOURCE_DIR}/../../../include
)

add_library(oscilloscope SHARED "${CMAKE_CURRENT_SOURCE_DIR}/../oscilloscope/oscilloscope.cpp")
add_library(oscilloscopemultichannel SHARED "${CMAKE_CURRENT_SOURCE_DIR}/../oscilloscopeMultiChannel/oscilloscopeMultiChannel.cpp")
add_library(thermometer SHARED "${CMAKE_CURRENT_SOURCE_DIR}/../thermometer/thermometer.cpp")

# Add dependencies to the nds3 library
#-------------------------------------
find_library(nds3_library NAMES nds3 PATHS ${LIBRARY_LOCATION})
target_link_libraries(oscilloscope ${nds3_library})
target_link_libraries(oscilloscopemultichannel ${nds3_library})
target_link_libraries(thermometer ${nds3_library})
