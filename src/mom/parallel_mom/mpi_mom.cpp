#include "mpi_mom.h"


void mpiPerformMoM(std::map<std::string, std::string> &const_map,
                   std::vector<Triangle> &triangles,
                   std::vector<Edge> &edges,
                   std::vector<Node<double>> nodes,
                   std::string file_name)
{
	// Some constants
	int zero = 0;
	int one = 1;
	int matrix_size = edges.size(); //TODO: Change

	// Get BLACS context
	int context;
	Cblacs_get(0, 0, &context);

	// Create BLACS grid
	//TODO: make variable grid size
	int total_proc_rows = 2;
	int total_proc_cols = 2;
	Cblacs_gridinit(&context, "Row-major", total_proc_rows, total_proc_cols);

	// Get proc coords
	int proc_row;
	int proc_col;
	Cblacs_gridinfo(context, &total_proc_rows, &total_proc_cols, &proc_row, &proc_col);

	//TODO: dynamic block size
	int block_size = 3;

	// Get the amount of rows for vrhs on each proc
	int num_vrhs_rows = numroc_(&matrix_size, &block_size, &proc_row, &zero, &total_proc_rows);
	int num_vrhs_cols = numroc_(&one, &block_size, &proc_col, &zero, &total_proc_cols);

 
     std::complex<double> *vrhs = new std::complex<double>[num_vrhs_rows * num_vrhs_cols]();
  

	mpi_fill_vrhs(vrhs,
           	      total_proc_rows,
                  total_proc_cols,
                  proc_row,
                  proc_col,
                  block_size,
                  const_map,
                  triangles,
                  edges);


    // Get amount of rows and cols for local zmn
    int num_zmn_rows = numroc_(&matrix_size, &block_size, &proc_row, &zero, &total_proc_rows);
    int num_zmn_cols = numroc_(&matrix_size, &block_size, &proc_col, &zero, &total_proc_cols);



    // Create local zmn matrix
    std::complex<double> *zmn = new std::complex<double>[num_zmn_cols * num_zmn_rows]();


    mpiFillZmn(zmn,
               num_zmn_rows,
               total_proc_rows,
               total_proc_cols,
               proc_row,
               proc_col,
               block_size,
               const_map,
               triangles,
               edges,
               nodes);

    mpiFillIlhs(zmn,
         vrhs,
         num_zmn_rows,
         num_vrhs_rows,
         matrix_size,
         block_size,
         context);


    std::complex<double> *gathered_ilhs;

    if(proc_col == 0 && proc_col == 0)
    {
        gathered_ilhs = new std::complex<double>[matrix_size];
    }

    // int send_row = 0;
    // int vrhs_local_index = 0;
    // for(int i = 0; i < matrix_size; i+= block_size)
    // {
    //     int num_rows = block_size;
    //     if(matrix_size - i < block_size)
    //     {
    //         num_rows = matrix_size - i;
    //     }

    //     if(send_row >= total_proc_rows)
    //     {
    //         send_row = 0;
    //     }

    //     if(proc_row == send_row && proc_col == 0)
    //     {
    //         Czgesd2d(context, num_rows, 1, vrhs + vrhs_local_index , matrix_size, 0, 0);

    //         vrhs_local_index += block_size;
    //     }
    //     if(proc_row == 0 && proc_col == 0)
    //     {
    //         Czgerv2d(context, num_rows, 1, gathered_ilhs + i, num_vrhs_rows, send_row, 0);   
    //     }
    //     send_row++;
    // }
	gatherIlhs(context, matrix_size, block_size, proc_row, proc_col, num_vrhs_rows,
			total_proc_rows, vrhs, gathered_ilhs);
    for(int i = 0; i < matrix_size; i++)
    {
        if(proc_row == 0 && proc_col == 0)
        {
            std::cout << gathered_ilhs[i] << std::endl;
        }
    }

}
