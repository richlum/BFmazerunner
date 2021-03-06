/*
 * DHeap is a heap with arity default value of 2.  Providing the constructor with
 * a different arity value changes the dheap allocation.  It is built on top
 * of vector template so it relies on underlying vector to resize as required
 * for capacity.  This means that size checking is removed from this code
 * to allow vector to take care of capacity adjustments.
 *
 * This DHeap is defaults to zero based first entry in vector but has logic
 * to handle one based vector that can be specified in constructor call.
 * 
 * Objects are evaluated based on values provided by Compare class for
 * sorting into heap.
 * 
 */

#include <vector>
#include <iostream>
#include "PQueue.h"
#include <cassert>


template <class Object, class Compare>
bool DHeap<Object, Compare>::isEmpty() const{
		return theHeap.size() == 0;	
	}

/*
 * This is a min heap so the minimum value is always at the root.
 */
template <class Object, class Compare>
const Object & DHeap<Object, Compare>::findMin() const{
		return theHeap[rootIndex];   // return object not index
	}

	
/*
 *  New items are inserted at the end of the heap and percolated
 *  up to correct position.
 *  @pre Object != null
 */
template <class Object, class Compare>
void DHeap<Object, Compare>::insert(const Object& o){
		//assert(!isFull());  // vector auto resizes, this stops it from happening.

		// place new object at end of tree/array
		// note that size-1 is index of last item in vector 
		// eg vector.back()=vector.size()-1
		theHeap.push_back(o);  
		int childIndex = theHeap.size()-1;// index of newly added item
		int parentIndex = getParentIndex(childIndex); //index of parent to newitem
		//int newPos =
		percolateUp(childIndex,parentIndex);
//debug
/*
		cout<< o <<" insert position " << newPos << "  heapsz = " << theHeap.size() << endl;
		printHeap();
*/
	}

/*
 * This is a min heap so, the minimum value object is at the
 * root of the tree.  As we remove the root of the tree, take
 * the item at the end of the tree and place it at the
 * root and allow it to percolate down to the correct position
 * @pre findmin()!=NULL; //there are objects in the heap to delete
 */
template <class Object, class Compare>
void DHeap<Object,Compare>::deleteMin(){
		//Object lastitem =
		theHeap[theHeap.size()-1];
/*
		cout << theHeap.back() << " = heap back" << endl;
		cout << theHeap.size() << " = heap size" << endl;
		cout << theHeap[theHeap.size()-1] << "value of last element " << endl;
*/
		theHeap[rootIndex]=theHeap.back();
		theHeap.pop_back();   // remove last element, decreasing size of heap by one
		//int result =
		percolateDown();
/*
		cout << result << "precolateDown result " << endl;
		cout << lastitem << " moved to top and percolated to position = " << result << endl;

		cout << "arraydump"<< endl;
		for (unsigned int i=0;i<theHeap.size();i++){
			cout<< "\t" << i << ", " << theHeap[i] << endl;
		}
*/
}

/*
 * Delete the min value object in the heap but place
 * a reference to it in the supplied minItem parameter
 * for caller to retrieve.
 * @pre findmin()!=NULL // there are objects to delete/retrieve
 */
template <class Object, class Compare>
void DHeap<Object, Compare>::deleteMin(Object& minItem){
		minItem = theHeap[rootIndex];
		deleteMin();
	}

/**
 * nuke the heap
 */
template <class Object, class Compare>
void DHeap<Object, Compare>::makeEmpty(){
		theHeap.clear();
	}

/*
 * report if the heap is full.  Should not require use of
 * this since underlying vector will manage capacity
 */
template <class Object, class Compare>
bool DHeap<Object, Compare>::isFull(){
		return (theHeap.capacity() - theHeap.size()) >0; 
	} 

/*
 * Algorithmn Based on Koffman for adding and deleting items to heap.
 * look for invariant violations in child parent relationships and
 * swap to positions to resolve as we push up newly added node from bottom
 * @invariant min tree ordering.
 * @invariant complete tree shape
 *
 */
template <class Object, class Compare>
int DHeap<Object, Compare>::percolateUp(int childIdx, int parentIdx){ 
		Compare comp;
/*
		cout << "percolateUP ( " << childIdx <<", " << parentIdx << ") = (c,p)" << endl;
		cout << "\tvalues(" << theHeap[childIdx] << ", " << theHeap[parentIdx] << ")" << endl;
		cout << "\tcomp = " << comp(theHeap[childIdx] , theHeap[parentIdx]) << endl;
*/
		while ((parentIdx >=rootIndex) && comp(theHeap[childIdx],theHeap[parentIdx])){
			//comp values with operator () from Compare
			//not at root and our value is less than parent
   			swapPosition ( childIdx, parentIdx); // exchange bins
                        childIdx = parentIdx;  // swapped, set current pos as child index
			parentIdx=getParentIndex(childIdx); //find new parent index and loop.
		}
		return childIdx; // index of newly created position for object
	}
	
/*
 * Algorithmn Based on Koffman for adding and deleting items to heap.
 * look for invariant violations in child parent relationships and
 * swap to positions to resolve as we push down node from the root
 * @invariant min tree ordering.
 * @invariant complete tree shape
 *
 */
template <class Object, class Compare>
int DHeap<Object, Compare>::percolateDown(){
//	cout << "percolate down" << endl;
		int parentIndex =rootIndex;	
		bool done = false ;
		Compare comp;
		while(!done){

			unsigned int minChildIndex = findMinChildIndex(parentIndex);
/*
			cout << minChildIndex << "minchildindex";
			cout << theHeap.size() << "heap size";
*/
			if (minChildIndex >= theHeap.size() ) {
				done = true;
			}else if (comp(theHeap[minChildIndex], theHeap[parentIndex])){ 
				//child is less than parent, swap positions so min is on top.
				swapPosition(parentIndex,minChildIndex);
				parentIndex=minChildIndex;
			}else{
				done = true;
				// arrived at place where parent is not less than us.
			}
		}
		return parentIndex;
	};

/*
 * given the index of a node, find all it's children
 * and return the index of the child with the minimum
 * value as evaluated by Compare class.  Index
 * greater than heap.size is a sign that no children
 * were found
 * @param the index of the object in the position for
 * which we want to identify the min value child
 * @return the index of the child of the object provided
 * with the minimum value as determined by compare class.
 * If there are no objects in the child node positions,
 * will still return the lowest index number child
 * of the provided node for the caller to check for bounds.
 */
template <class Object, class Compare>
int DHeap<Object, Compare>::findMinChildIndex(int parentIndex){
		Compare comp;
		unsigned int* childIndexes = NULL;
		childIndexes = new unsigned int[arity];
		findChildIndexes(parentIndex, (int* )childIndexes);
		int min = childIndexes[0] ; // assume first is min and compare to all other siblings
		for (int i=1; i<arity; i++){
			if (childIndexes[i]<theHeap.size()){
/*
				cout << "comp i= " << i << "min indx = " << min << endl;
				cout << theHeap[childIndexes[i]] << " , " << theHeap[min]<< endl;
*/
				if (comp(theHeap[childIndexes[i]],theHeap[min]))
					min = childIndexes[i];  // index for childIndexes
			}
		}
		int result = min; //result is theHeap index
		delete childIndexes;
		return result;
		// if any are less than heap.size, return min
		// if only the first is less than heap.size, return it
		// if none are less than heap.size, return the lowest index
		// and let caller detect it out of range.
	}

/*
 * given the index of a parent, find the indexes of it's
 * children.  Purely a function of the arity of the heap
 * @param parentIndex of the node we want the children indexes of
 * @param childIndex - array of indexes of the childent that belong
 * to the parentIndex. Populated by this method.
 */
template <class Object, class Compare>
void DHeap<Object, Compare>::findChildIndexes(int parentIndex,int* childIndexes){
// binary with zero based vector underlying
//		childIndexes[0] = parentIndex*2+1;
//		childIndexes[1] = parentIndex*2+2;
	if (rootIndex == 0){  // logic for 0 based vector
		childIndexes[0] = parentIndex*arity + 1;
		for(int i=1;i<arity;i++){
			childIndexes[i]=childIndexes[i-1]+1;
		}
	}else if (rootIndex ==1) { // logic for 1 based vector
		// untested
		childIndexes[0] = (parentIndex-1)*arity +2;
		for(int i=1;i<arity;i++){
			childIndexes[i]=childIndexes[i-1]+1;
		}
	}else{  // dont know how to handl any other options.
		for (int i=0;i<arity;i++)childIndexes[i]=-1;
		// blow things up
	}

}

/*
 * given an index in the tree, find the index of its parent
 * Purely a function of tree arity
 * @param holeAddress is the heap index of the item we want
 * to know the parent index of.
 * @return index of the item that occupies the parent position
 * of provided index.
 */
template <class Object, class Compare>
int DHeap<Object, Compare>::getParentIndex(int holeAddress){
	// hardcode for airty 2 = btree initially, 
		int result =rootIndex;
	//	result = (holeAddress-1)/2; // zero based indexes
		if (rootIndex == 0){ // zero based array
				result = (holeAddress-1)/arity ;
		}else if (rootIndex ==1) { // one based array
				// untested
				result = ((holeAddress-2)/arity)+1;
		}else{  //dont know how to handle anything else
				result =-1;
				// blow things up
		}
		return result;
	}

/*
 * Switch the positions in the heap of the two provided
 * objects
 * @param index of one item in heap
 * @param index of the other item in the heap to swap
 */
template <class Object, class Compare>
void DHeap<Object, Compare>::swapPosition(int laddr, int raddr){
/* debug
		cout << "swapping (" << laddr << ", " << raddr << ")  " << endl;
		cout << "\t" << theHeap[laddr] << ", " << theHeap[raddr] << ")" << endl;
*/
		Object temp = theHeap[laddr];
		theHeap[laddr]=theHeap[raddr];
		theHeap[raddr]= temp; //using default copy constructor. Warning: will not 
	       				// work properly if Object contains pointers??	
//		cout << "\t" << theHeap[laddr] << ", " << theHeap[raddr] << ")" << endl;
}
/*
 * utility for debugging to print out heap contents
 */
template <class Object, class Compare>
void DHeap<Object, Compare>::printHeap(){
	Compare comp;
	cout << "----printing heap ----Arity = " << arity << endl ;
	for(unsigned int i=rootIndex; i<theHeap.size(); i++) {
		cout << "\t" << i << "\t" << theHeap[i] << "\t" ;
		//(theHeap[i])->print(cout);  
		// this works for mazenode but breaks for sort since int has no print method
		cout << "  value = " << comp.nodeValue(theHeap[i]);
		cout << endl;
	}
}

