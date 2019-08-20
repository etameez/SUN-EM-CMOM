set(dgfm_helper_sources
		${CMAKE_CURRENT_SOURCE_DIR}/dgfm_helpers.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/dgfm_weights.cpp)

target_sources(cmom PRIVATE ${dgfm_helper_sources})
target_include_directories(cmom PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")