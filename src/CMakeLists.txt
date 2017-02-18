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

add_library(OpenTimer ${SOURCE})
target_include_directories(OpenTimer PRIVATE ${GLOG_INCLUDE_DIRS})
target_link_libraries(OpenTimer PRIVATE OpenTimerLefDef INTERFACE ${GLOG_LIBRARIES})

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
)

install(TARGETS OpenTimer DESTINATION OpenTimer/lib )
file(GLOB HEADERS "*/*.h*" )
install(FILES ${HEADERS} DESTINATION OpenTimer/include )
