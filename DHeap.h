// This file contains the declaration of the concrete class
// Dheap which extends the base class
// PriorityQueue. 

#ifndef _DHEAP_H
#define _DHEAP_H

#include <vector>
#include <iostream>
#include "PQueue.h"

using namespace std;

// The Dheap class is a heap with each parent having D children
// as specified by optional arity parameter in constructor.
// It extends the abstract class PriorityQueue
// It is based on Vector as the underlying structure and 
// can optionally use 0 based or 1 based vector (constructur parameter)

// This is a min heap
// @invariant: parents key is less than all children key. Min is always at top
// @invariant: structure is nearly complete tree. Depth is always O(log n).
//            next open location is end of Vector.
template <class Object, class Compare>
class DHeap : public PriorityQueue<Object, Compare> {
public:


  DHeap(){
	arity = 2;
	rootIndex=0;
	theHeap = vector<Object>();
	cout << "Using Default Heap Arity = " << arity << endl;
	cout << "Using rootIndex = " << rootIndex << endl;
	};
 // allow calling code to specify desired arity 
  DHeap(int requestedArity){
      	if (requestedArity >=2) {
		this->arity = requestedArity;
	}else{
	    	cout << "airty must be >=2, using default  " << endl;
		this->arity = 3;
	}
	rootIndex=0;
	theHeap = vector<Object>();
	cout << "setting Heap Arity = " << arity << endl;
	cout << "Using rootIndex = " << rootIndex << endl;

	};          
// allow user code to specify arity and rootIndex of underlying
// vector implementation.  
  DHeap(int requestedArity, int requestedRootIndex){
      	if (requestedArity >=2) {
		this->arity = requestedArity;
	}else{
	    	cout << "airty must be >=2, using default  " << endl;
		this->arity = 3;
	}
	if(requestedRootIndex == 1){
	    	rootIndex=1;
	    }else{ // if we are given illegal rootIndex, just use zero
		rootIndex=0;
		if (requestedRootIndex!=0) cout << "rootIndex must be zero or one, using default"<< endl;
	    }	
	theHeap = vector<Object>();
	cout << "setting Heap Arity = " << arity << endl;
	cout << "Using rootIndex = " << rootIndex << endl;

	};          

  ~DHeap() { 
	makeEmpty();
	//delete theHeap;	
	 };    

  // Const functions

  bool isEmpty() const;               // True iff the pqueue is empty
  const Object & findMin() const;     // Returns unchangeable reference to
                                      // minimum elment. Should _not_ be 
                                      // used to alter the minimum
                                      // element in any way.
  
  // Non-const functions

  void insert(const Object & x);      // Inserts a copy of x into the pqueue
  void deleteMin();                   // Deletes the minimum element from 
                                      // the dheap
  void deleteMin(Object & minItem);   // Deletes the minimum element
                                      // from the dheap and puts a copy 
                                      // of it into minItem.
  void makeEmpty();                   // Deletes all elements from the
                                      // dheap
  bool isFull();			// return true if heap is full. 
  					// use of vector means we dont need to manually check capcity
  					// 
  void printHeap();
private:
  int arity;  // update with constructor 
  int rootIndex;  // the root of the tree = min
  std::vector<Object> theHeap;

  // Some helper functions for internal use
  int percolateUp(int childIndex, int parentIndex);  // returns final position of new object that 
                                                     // was added at the end of the tree.
  int percolateDown(); //for relocating last object into new hole at root created by deleteMin
  int getParentIndex(int childIndex);
  int findMinChildIndex(int parentIndex);
  void swapPosition(int child, int parent);
  void findChildIndexes(int parentIndex, int* childIndexes); 


};


//  implementation code to make templates work.
#include "DHeap.cpp"


#endif
