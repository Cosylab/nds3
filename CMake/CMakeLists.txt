cmake_minimum_required(VERSION 2.6)

project (nds3)

set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++0x -Wall -Wextra -pedantic -fPIC -pthread -fvisibility=hidden -fvisibility-inlines-hidden" )
set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -fPIC -Wl,-as-needed" )

add_definitions(-DNDS3_DLL)
add_definitions(-DNDS3_DLL_EXPORTS)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)

file(GLOB nds_include_interface "${CMAKE_CURRENT_SOURCE_DIR}/../include/nds3/*.h")
file(GLOB nds_include_implementation "${CMAKE_CURRENT_SOURCE_DIR}/../include/nds3impl/*.h")
file(GLOB nds_sources "${CMAKE_CURRENT_SOURCE_DIR}/../src/*.cpp")

add_library(nds3 SHARED  ${nds_sources})

target_link_libraries(nds3 rt dl pthread)

set( CPACK_PACKAGE_NAME "NDS3Core")
set( CPACK_PACKAGE_VENDOR "Cosylab d.o.o.")
set( CPACK_PACKAGE_DESCRIPTION_SUMMARY "Nominal device support for control systems (core libraries)")
set( CMAKE_INSTALL_PREFIX "/usr")

install(TARGETS nds3 LIBRARY DESTINATION lib64 COMPONENT "Shared library")
install(FILES ${nds_include_interface} DESTINATION include/nds3 COMPONENT "Included files")
install(FILES ${nds_include_implementation} DESTINATION include/nds3impl COMPONENT "Included files for control system developers")

include(CPack)


