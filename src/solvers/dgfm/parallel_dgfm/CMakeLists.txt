if(MPI)
	set(parallel_dgfm_sources
			${CMAKE_CURRENT_SOURCE_DIR}/mpi_dgfm.cpp
        	${CMAKE_CURRENT_SOURCE_DIR}/mpi_idgfm.cpp)

	target_sources(cmom PRIVATE ${parallel_dgfm_sources})
	target_include_directories(cmom PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
endif(MPI)