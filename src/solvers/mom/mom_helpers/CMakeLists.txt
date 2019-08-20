set(mom_helpers_sources
		${CMAKE_CURRENT_SOURCE_DIR}/quadrature.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/vrhs.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/zmn_by_face.cpp)

target_sources(cmom PRIVATE ${mom_helpers_sources})
target_include_directories(cmom PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")