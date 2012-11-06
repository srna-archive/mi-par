#include <iostream>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <ctime>

#include "Instance.h"
#include "Solution.h"
#include "Worker.h"

#include "mpi.h"

using namespace std;

int usage(char *bin);

int main(int argc, char **argv)
{
	if(argc < 2)
		return usage(argv[0]);

	try
	{
		int my_rank = -1, p = -1;
		double t1, t2;
		
		// Init MPI
		MPI_Init(&argc, &argv);
		
		t1 = MPI_Wtime();
		
		// Get my rank
		MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

		// Get process count
		MPI_Comm_size(MPI_COMM_WORLD, &p);
		
		Instance *pi = NULL;
	
		// Process with rank 0 reads the instance and sends it to others
		if(my_rank == 0)
			pi = new Instance(argv[1]);
		
		// Wait for all
		//MPI_Barrier(MPI_COMM_WORLD);
		
		// Let's do the work!
		Worker w(pi);
		w.work();
		
#ifdef DEBUG
		cout << "Worker of process " << my_rank << " has finished Worker::work()." << endl;
#endif
		
		MPI_Barrier(MPI_COMM_WORLD);
		
		t2 = MPI_Wtime();
		
		// Write the best solution
		if(my_rank == 0)
		{
			cout << "The shortest superstring is:" << endl;
			cout << *w.getSolution() << endl;
			cout << "Time: " << t2-t1 << endl;
		}
		
		// Finalize MPI
		MPI_Finalize();
	}
	catch (const char * e)
	{
		cout << e << endl;
		// TODO: Send error broadcast to terminate others
		
		// Finalize MPI
		MPI_Finalize();
		return 1;
	}

	return 0;
}

int usage(char *bin)
{
	cout << "Usage: " << bin << " file" << endl;
	cout << "file\tstring\t\t\tInput file" << endl;
	return 1;
}
