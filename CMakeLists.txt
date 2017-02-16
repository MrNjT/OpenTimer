cmake_minimum_required(VERSION 3.2.2)

project(Open-Timer)
set(CMAKE_CXX_STANDARD 11)

include(ExternalProject)


set(3RDPARTY_PATH ${CMAKE_CURRENT_SOURCE_DIR}/3rd-party)

ExternalProject_Add(glog-0.3.3
    SOURCE_DIR ${3RDPARTY_PATH}/glog-0.3.3
    CONFIGURE_COMMAND "./configure" --prefix=${3RDPARTY_PATH}/glog-0.3.3
    BUILD_COMMAND make
    BUILD_IN_SOURCE 1
)

ExternalProject_Add_Step(glog-0.3.3 autoreconf
    WORKING_DIRECTORY ${3RDPARTY_PATH}/glog-0.3.3
    COMMAND autoreconf -f -i
)

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
install(FILES ${HEADERS} DESTINATION include)

add_subdirectory(src)
