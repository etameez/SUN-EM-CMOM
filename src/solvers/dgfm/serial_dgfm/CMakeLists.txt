set(serial_dgfm_sources
		${CMAKE_CURRENT_SOURCE_DIR}/dgfm.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/dgfm_row.cpp)

target_sources(cmom PRIVATE ${serial_dgfm_sources})
target_include_directories(cmom PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")