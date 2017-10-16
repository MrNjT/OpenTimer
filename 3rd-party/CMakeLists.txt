file(GLOB SOURCES "DEF/*.cpp" "LEF/*.cpp" )
add_library(OpenTimerLefDef ${SOURCES})
install(TARGETS OpenTimerLefDef DESTINATION lib/OpenTimer)
