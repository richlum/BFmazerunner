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
  // Holder for numbers from input
  int num;

  // Construct a priority queue, specifying arity of 3 and 
  // to use zero based array underneath heap.
  DHeap<int, less_int> * pq = new DHeap<int, less_int>(3,0);

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
    cerr << "Usage: sort [n]\nIf you supply n, sorts n random values.\n" 
	    << "If you do not supply n, sorts numbers from standard input,\n"
	    << "terminates on a 0 or an end of file." << endl;
    exit(1);
  }
  else if (argc == 2) {
    // Generate random numbers to put in the queue
    int numToGenerate = atoi(argv[1]);

    // Seed the random number generator
    srandom(time(NULL));

    // Put in some numbers
    for (int i = 0; i < numToGenerate; i++) {
	 // Modding by numToGenerate keeps them in a reasonable range.
	 pq->insert(random() % numToGenerate);
    }
  }
  else {
    // Read numbers from standard input until you read a 0.
    while (cin >> num && num != 0) {
	 pq->insert(num);
    }
  }

  // Print out each number from the priority queue in increasing
  // order. (How would we get decreasing order?)
  while (!pq->isEmpty()) {
    int i;
    pq->deleteMin(i);
    cout << i << " ";
  }
  cout << endl;

  return 0;
}
