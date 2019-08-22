set(serial_mom_sources
		${CMAKE_CURRENT_SOURCE_DIR}/fill_ilhs.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/fill_vrhs.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/fill_zmn.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/mom.cpp)

target_sources(cmom PRIVATE ${serial_mom_sources})
target_include_directories(cmom PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")