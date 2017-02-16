set(SOURCE )

add_subdirectory(debug)
add_subdirectory(dist)
add_subdirectory(dump)
add_subdirectory(event)
add_subdirectory(generic)
add_subdirectory(iterator)
add_subdirectory(liberty)
add_subdirectory(log)
add_subdirectory(opentimer-stamp)
add_subdirectory(sdc)
add_subdirectory(shell)
add_subdirectory(spef)
add_subdirectory(timer)
add_subdirectory(verilog)

link_directories(${3RDPARTY_PATH}/glog-0.3.3/lib/)
add_library(OpenTimer SHARED ${SOURCE})
target_link_libraries(OpenTimer PUBLIC glog)

target_include_directories(OpenTimer
  PUBLIC debug
  PUBLIC dist
  PUBLIC dump
  PUBLIC event
  PUBLIC generic
  PUBLIC iterator
  PUBLIC liberty
  PUBLIC log
  PUBLIC opentimer-stamp
  PUBLIC sdc
  PUBLIC shell
  PUBLIC spef
  PUBLIC timer
  PUBLIC verilog
  PUBLIC ${3RDPARTY_PATH}/DEF
  PUBLIC ${3RDPARTY_PATH}/LEF
  PUBLIC ${3RDPARTY_PATH}/glog-0.3.3/include
)

install(TARGETS OpenTimer DESTINATION lib )
file(GLOB HEADERS "*/*.h*" )
install(FILES ${HEADERS} DESTINATION include )
