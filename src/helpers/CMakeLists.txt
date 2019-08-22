set(helper_sources
        ${CMAKE_CURRENT_SOURCE_DIR}/globals.cpp)

target_sources(cmom PRIVATE ${helper_sources})
target_include_directories(cmom PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")