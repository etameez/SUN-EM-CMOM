cmake_minimum_required (VERSION 3.1)
set (CMAKE_CXX_STANDARD 11)

project(cmom CXX C Fortran)

option(MPI "Compile with MPI" OFF)


set(SOURCES lib/args/args.hxx)

if(MPI)
    MESSAGE("COMPILING WITH MPI")
    add_definitions(-DPARALLEL)

    find_package(MPI REQUIRED)
    include_directories(${MPI_INCLUDE_PATH})

    add_executable(cmom ${SOURCES})
     
    set(CMAKE_CXX_FLAGS "-O3 ")
    
    target_link_libraries(cmom      ${MPI_LIBRARIES}
                                    -L/${CMAKE_CURRENT_SOURCE_DIR}/lib/scalapack
                                    -lscalapack
                                    -lreflapack
                                    -lrefblas
                                    -ltmg
                                    -L/usr/bin
                                    -lgfortran)
    set_target_properties(cmom PROPERTIES OUTPUT_NAME "cmompi")

else()
    MESSAGE("COMPILING WITHOUT MPI")
    add_executable(cmom ${SOURCES})
     
   	set(CMAKE_CXX_FLAGS "-O3 ")
 
    target_link_libraries(cmom   -L/${CMAKE_CURRENT_SOURCE_DIR}/lib/scalapack
                                -lreflapack
                                -lrefblas
                                -ltmg 
                                -L/usr/bin
                                -lgfortran)

endif(MPI)

add_subdirectory(src)

unset(MPI CACHE)
