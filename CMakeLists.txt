cmake_minimum_required (VERSION 3.1)
set (CMAKE_CXX_STANDARD 11)

project(cmom CXX C Fortran)

option(MPI "Compile with MPI" OFF)


set(SOURCES lib/args/args.hxx)

if(MPI)
    MESSAGE("COMPILING WITH MPI")
 #   set_target_properties(
 #   a 
 #   PROPERTIES 
 #       OUTPUT_NAME "myname"
 #       SUFFIX ".myext"
)

else()
    MESSAGE("COMPILING WITHOUT MPI")
    add_executable(cmom ${SOURCES})

endif(MPI)

add_subdirectory(src)

unset(MPI CACHE)
