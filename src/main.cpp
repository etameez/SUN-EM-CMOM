//**********************************
#include <complex>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>

#include "../lib/args/args.hxx"
#include "file_io/mom_file_reader.h"
#include "file_io/mom_file_writer.h"

#ifndef PARALLEL

#include "solvers/mom/serial_mom/mom.h"
#include "solvers/dgfm/serial_dgfm/dgfm.h"
#endif

#ifdef PARALLEL
// #include "solvers/mom/parallel_mom/mpi_mom.h"
#include "solvers/dgfm/parallel_dgfm/mpi_dgfm.h"
#include <mpi.h>
#endif

//**********************************

int main(int argc, char **argv)
{

	args::ArgumentParser parser("CMoM: C++ Method of Moments Solver", "");
	args::HelpFlag help(parser, "help", "Display this menu", {'h', "help"});
	args::Positional<std::string> file_name_arg(parser, "file_path", "The path to the .mom file");
    args::Group group(parser, "");

    args::Flag cbfm(group, "cbfm", "DONT USE", {"cbfm"}); 
    args::Flag svd(group, "svd", "DONT USE", {"svd"});
    args::Flag dgfm(group, "dgfm", "USE DGFM Solver", {"dgfm"});


    //----------------------------------------
    // Command line argument parser
    // Used to print help comman -h
	try
    {
        parser.ParseCLI(argc, argv);
    }
    catch (const args::Completion& e)
    {
        std::cout << e.what();
        return 0;
    }
    catch (const args::Help&)
    {
        std::cout << parser;
        return 0;
    }
    catch (const args::ParseError& e)
    {
        std::cerr << e.what() << std::endl;
        std::cerr << parser;
        return 1;
    }
    //----------------------------------------

    #ifdef PARALLEL
    MPI_Init(NULL, NULL);
    int size;
    int rank;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
    #endif

    // Read the .mom file


    // TODO FIX THIS PROPERLY
    bool domain_decomp = false;
    if (cbfm || dgfm)
    {
        domain_decomp = true;
    }
    MoMFileReader reader(args::get(file_name_arg), domain_decomp);

    
    // // Create the array to store the MoM solution
    std::complex<double> *ilhs; 

    if (dgfm)
    {
        #ifndef PARALLEL
        ilhs = new std::complex<double>[reader.edges.size()]();
        performDGFM(reader.const_map,
                    reader.label_map,
                    reader.triangles,
                    reader.edges,
                    reader.nodes,
                    reader.excitations,
                    ilhs);
                  
        writeIlhsToFile(ilhs, reader.edges.size(), args::get(file_name_arg));   
        delete [] ilhs;
        #endif
        
        #ifdef PARALLEL
        if(rank == 0)
        {
            ilhs = new std::complex<double>[reader.edges.size()];
        }

        mpiPerformDGFM( reader.const_map,
                        reader.label_map,
                        reader.triangles,
                        reader.edges,
                        reader.nodes,
                        reader.excitations,
                        ilhs); 

        if(rank == 0)
        {
            // Write the solution to a .sol file
            writeIlhsToFile(ilhs, reader.edges.size(), args::get(file_name_arg));   
            
            // Cleanup
            delete [] ilhs;
        } 

        #endif
    }



    #ifdef PARALLEL
    MPI_Finalize();
    #endif

    return 0;
}

