#include <iostream>
#include <cstdlib>
#include <ctime>
#include "DHeap.h"
#include <time.h>

using namespace std;

// A comparator class for priority queues. This returns true if and
// only if lint is less than rint.
struct less_int {
	bool operator() (const int & lint, const int & rint) const
	{ return lint < rint; }
	int nodeValue(const int & thing) const
	{ return thing;} // for parallell to mazeNodeCompare::nodeValue
};

int main(int argc, char * argv[])
{
	time_t startTime;
	time_t endTime;
	int arity[] = {2,3,4,16,128,256,512,1024};
	int sizeOfArityArray = sizeof(arity)/sizeof(int);



	// Check the arguments.
	//
	// If there is an argument, assume that it's a number and that it
	// indicates a number of random values to generate and sort.
	//
	// If there is no argument, assume that the user will provide
	// numbers, terminating with a 0 or end of file, from standard
	// input.
	if (argc > 2) {
		// Error: give usage message.
		cerr << "Usage: sort [n]\nIf you supply n, sorts n random values.\n"  << endl;
		cerr << "using different arity levels" << endl;
		exit(1);
	}

	// Seed the random number generator
	srandom(time(NULL));
	for(int iteration=0;iteration < sizeOfArityArray; iteration++ ){
	    // DHeap specified with given arity and using 1 based array logic
		DHeap<int, less_int> * pq = new DHeap<int, less_int>(arity[iteration],1);
		if (argc == 2) {
			// Generate random numbers to put in the queue
			int numToGenerate = atoi(argv[1]);

			// start timer for performance check
			time(&startTime);
			// Put in some numbers
			for (int i = 0; i < numToGenerate; i++) {
				// Modding by numToGenerate keeps them in a reasonable range.
				pq->insert(random() % numToGenerate);
			}
		}


		// Print out each number from the priority queue in increasing
		// order. (How would we get decreasing order?)
		int counter =0;
		while (!pq->isEmpty()) {
			int i;
			counter++;
			pq->deleteMin(i);
			if (counter<1000) // only print out first 10k sorted numbers
				cout << i << " ";
		}
		delete pq;
		cout << endl;
		// stop timer
		time(&endTime);
		cout << "------------------------------------------------------------------" << endl;
		cout <<" Arity =" <<  arity[iteration] << " execution time : " << difftime(endTime,startTime);
		cout <<endl;
		cout << "------------------------------------------------------------------" << endl;
	}
	return 0;
}
