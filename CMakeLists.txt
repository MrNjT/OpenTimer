cmake_minimum_required(VERSION 3.2.2)
project(Open-Timer)

include(ExternalProject)

set(CMAKE_CXX_STANDARD 14)
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
set(3RDPARTY_PATH ${CMAKE_CURRENT_SOURCE_DIR}/3rd-party)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package(Glog 0.3.3 REQUIRED)

find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

add_definitions(-DPACKAGE_STRING="OpenTimer 1.0.5")
add_definitions(-DPACKAGE_NAME="OpenTimer")
add_definitions(-DPACKAGE_TARNAME="OpenTimer")
add_definitions(-DPACKAGE_VERSION="1.0.5")
add_definitions(-DPACKAGE_STRING="OpenTimer 1.0.5")
add_definitions(-DPACKAGE_BUGREPORT="twh760812@gmail.com")
add_definitions(-DPACKAGE_URL="<http://web.engr.illinois.edu/~thuang19>")


file(GLOB HEADERS "${3RDPARTY_PATH}/*/*.h*" )
install(FILES ${HEADERS} DESTINATION OpenTimer/include)

add_subdirectory(src)
add_subdirectory(main)
add_subdirectory(3rd-party)
