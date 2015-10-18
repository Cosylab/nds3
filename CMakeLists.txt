cmake_minimum_required(VERSION 2.8)

project (nds3)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -Wall -Wextra -Wpedantic -pthread --coverage")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++0x -Wall -Wextra -pedantic -fPIC -pthread -fvisibility=hidden -fvisibility-inlines-hidden" )
SET( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -fPIC -Wl,-as-needed" )

add_library(nds3 SHARED 
    src/base.cpp
    src/baseImpl.cpp
    src/dataAcquisition.cpp
    src/dataAcquisitionImpl.cpp
    src/delegate.cpp
    src/exceptions.cpp
    src/factory.cpp
    src/factoryBaseImpl.cpp
    src/interfaceBaseImpl.cpp
    src/node.cpp
    src/nodeImpl.cpp
    src/port.cpp
    src/portImpl.cpp
    src/pvBase.cpp
    src/pvBaseImpl.cpp
    src/pvHoldDelegate.cpp
    src/pvHoldDelegateImpl.cpp
    src/stateMachine.cpp
    src/stateMachineImpl.cpp
    src/logStreamGetterImpl.cpp
    src/pvBaseIn.cpp
    src/pvBaseOut.cpp
    src/pvBaseOutImpl.cpp
    src/PVBaseInImpl.cpp
    src/pvDelegateIn.cpp
    src/pvDelegateOut.cpp
    src/pvDelegateOutImpl.cpp
    src/pvDelegateInImpl.cpp
    src/pvVariableOut.cpp
    src/pvVariableIn.cpp
    src/pvVariableInImpl.cpp
    src/pvVariableOutImpl.cpp
    src/ndsFactoryImpl.cpp)

target_link_libraries(nds3 rt dl pthread)

