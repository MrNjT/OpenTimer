include_directories(
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
link_libraries(OpenTimer)
add_executable(ot_dist_agent ot_dist_agent.cpp)
add_executable(ot_dist_master ot_dist_master.cpp)
add_executable(ot_main ot_main.cpp)
add_executable(ot_tau15 ot_tau15.cpp)
add_executable(ot_tau16 ot_tau16.cpp)

install(TARGETS ot_dist_agent ot_dist_master ot_main ot_tau15 ot_tau16 DESTINATION OpenTimer/bin)
