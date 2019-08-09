
add_subdirectory(data_structures)
add_subdirectory(file_io)

set(main_sources
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

target_sources(cmom PRIVATE ${main_sources})
target_include_directories(cmom PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")