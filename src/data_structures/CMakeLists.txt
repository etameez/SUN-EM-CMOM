
set(data_structure_sources
        ${CMAKE_CURRENT_SOURCE_DIR}/edge.h
        ${CMAKE_CURRENT_SOURCE_DIR}/excitation.h
        ${CMAKE_CURRENT_SOURCE_DIR}/label.h
        ${CMAKE_CURRENT_SOURCE_DIR}/node.h
        ${CMAKE_CURRENT_SOURCE_DIR}/solution_setup.h
        ${CMAKE_CURRENT_SOURCE_DIR}/triangle.h)

if(MPI)
   list(APPEND data_structure_sources cblacs_config.h)
endif(MPI)

target_sources(cmom PRIVATE ${data_structure_sources})
target_include_directories(cmom PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")