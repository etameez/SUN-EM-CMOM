set(file_io_sources
        ${CMAKE_CURRENT_SOURCE_DIR}/mom_file_reader.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/mom_file_writer.cpp)

target_sources(cmom PRIVATE ${file_io_sources})
target_include_directories(cmom PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")