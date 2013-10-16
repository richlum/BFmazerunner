Student Name #1:		Richard Lum
----------------------------------------------------------------------

Acknowledgment of assistance:
Text:		Koffman, Objects, Abstractions, DataStructures
		esp for heap algorithmns 
reference: 	to help resolve how to use Abstract base class
http://stackoverflow.com/questions/650091/using-an-abstract-class-in-c

Summary
- implemented Best First Maze Runner to display 
	1. DHeap based priority Queue
	2. UnsLLPQueue based priority Queue
	3. RandomMazeRunner
- Compare.h implements SquareMazeNodeCompare using manhanttan
	distance algorithm provided.
- DHeap based priority queue uses ExtraNodeInfo data structure 
	since I finished most of the work in that area by the time
	the project annoucement indicated that this was outdated
	Because ExtraNodeInfo is designed to capture forward
	path, it was more work to capture solution path than
	a setParentPath since we could set parentPaths as we
	put nodes into the priorityQ whereas we cannot set
	the next in solution until after we have found the nextnode.
- Implemented MazeNode within MazeRunner and PriorityQueue, only
	casting to SquareMazeNode in SquareMazeNodeCompare 
- Implemented a optional sort (for solo) to show impact of arity.  
- Implemented both zero based and 1 based underlying vector for sort.
- tested against no solution. Passes but random maze runner runs 
	infinitely.


----------------------------------------------------------------------

Files in your submission:

[Add documentation to this list:]

* README.txt: this file
* 
* BFMazeRunner.[h,cpp] BestFirst implementation files.  Largely identical
	RandomMazeRunner with some functionality migrated from solveMaze
	into pickNextNode.  pickNextNode does all the work of marking
	visit states, and getting neighbours inserted into priorityQ.
	New method added to address the build of solution path. Since
	pickNextNode produces nonAdjacent nodes for search, it gets much
	harder to keep a solution path up todate during exploration.
	pointToMe is recursive search of visited nodes back from exit
	node to get the solution path out of the visited nodes.
	Structured to accept a pointer to any PriorityQueue derived
	class as a parameter to constructor.
* Compare.h  : modified to implement the Modified Manhattan distance
	algorithm for getting node values.
* DHeap.[h,cpp] : heavily based on UnsLLPQueue implementation structure.
	Implements Abstract PriorityQueue as a D heap with variable arity
	that can be specified on Constructor call. Utilizes vector as
	underlying data structure and relies on vector for automatic
	sizing adjustments.  Implemented as a zero based array by
	default but also has working logic for one based array that 
	can be specified by constructor call that takes arity and/or
	base for the array.  sort, sort2, sort3 alternately exercise
	different arity and array base values.
* runmaze.cpp - lightly modified to instantiate a DHeap, a UnsLLPQueue and
	pass them  to BFMazeRunner() for test runs.  Also retained the
	RandomMazeRunner for solution comparision.
	curently set to use a Default DHeap (arity 2, zero based root index).
* Makefile - modified as required
* sort.cpp-lightly modified to use DHeap Priority Queue with arity 3 using
	zero based vector underneath Heap
* sort2.cpp - customized to generate specified qty of random numbers and pass it into
	DHeap for sorting.  Time is reported for each pass and the arity
	is adjusted to take values {2,3,4,16,128,256,512,1024}
	Uses 1 based array underneath DHeap.
	removed ability to take piped input
* sort3.cpp - removes echo of values and sets up timers for arity values
	{2,3,4,6,8,12,24}.  Uses rootIndex = 1 for vector
* maze10.txt - example of case where BFMazeRunner will not pick shortest
	path.

----------------------------------------------------------------------

High-level description , including any problems or surprises:

DHeap is implemented as a Heap with specified arity.  It is built on top 
of vector data structure and is implemented as a zero based array. Zero
based array is complete and works correctly.  
Dheap  has datafillable structure to use 1 based array but there are still
errors in the logic that havent been fully corrected. around the the size
of the vector being 1 when 'empty'.

Previous MazeRunners all produced physically adjacent nodes for the next
node to process.  The SquareMazeNodeCompare calculates roughly based on 
local distance to exit node.  This results in occassionally getting several
non adjacent nodes in time sequence.  This means we cannot build a solution
path based upon sequence visted. The fact the the ExtraNodeInfo
captures nextInSolution creates a situation where we cannot fill
the in the information at the time we look at the node since it can 
point only to one of its multiple children.

To solve this, I used a backward search of visited nodes once 
we find the exit node. The pointToMe method that builds recursively 
until it finds the startNode.  

NOTE: I used the ExtraNodeInfo data structure as I had largely completed
the work in that section  by the time that we were notified that 
these project files were old.  I think the setParentPath of prior 
project would have made the pointToMe method obsolete becase we could
setParentPath at time of insertion to priorityQ.   

ExtraNodeInfo data structure appears to also be used by the 
visualizer code. runmaze would run to completion
without visualizer but would seg fault with visulizer. 
Presence of data in the ExtraNodeInfo->nextInSolution 
prevents proper calculation of solution
path leading to nulls in the solution path being printed.  I  
delete, and add ExtraNodeInfo and initialize all nextInSolution 
to NULL for all MazeNodes prior to building solution path, For
solution path to be built as I used the presence 
of data as an indicator that we have processed the node and I suspect
visualizer was adding extraNodeInfo although the debugger would not show
me where the data change was happening. 
(see BFMazeRunner::pointToMe)


----------------------------------------------------------------------

Answers to questions:

Best First Algorithm is providing local node values based on absolute
distance from the exit node. It will make local decisions that
improve that value.  As such, you can construct a maze that allows
BFMazerunner to get closer to exit and then force it to go back out.
Essentially, if the shortest path is not a direct path, Best First will
not find it.  maze10.txt shows a sample maze that does this by allowing
the mazerunner to wind back and forth trying to stay as close to the
exit node as possible whereas the shortest path is along the periphery
furthest away from the exit node.  As a result best first winds along
a snake path while the shortest path is actually along the outside
edge.

UnsLLPQueue.cpp makeEmpty removes nodes based on the minimum value and 
deletes them.   Since it is implemented as a double linked list circular 
list we could simply start with any node and delete it's next node. This 
avoids the cost of searching for min nodes in each pass.

eg
	getany ListNode
		// loop until we point to ourself
	while ListNode->next != ListNode 
		// point to node on the far side of  next
		ListNode->next = ListNode->next->next; 
		delete ListNode->next; 
		//  repair broken reverse link to point 
		//  at us instead of deleted node
		listNode->next->previous = ListNode; 
	}
	delete ListNode;  // delete the last node

OPTIONAL : sort with different arity since working solo.
sorting using different arity heaps.
sort2 is written to accept size of random numbers to input and displays the top
1000 numbers but times the full input and extraction of all numbers.  It is set to 
do this for arity values {2,3,4,16,128,256,512,1024}
results for 1000000
Arity =2 execution time : 2
Arity =3 execution time : 2
Arity =4 execution time : 2
Arity =16 execution time : 3
Arity =128 execution time : 11
Arity =256 execution time : 21
Arity =512 execution time : 35
Arity =1024 execution time : 62

at this level of workload, there was no performance difference for arity 2,3 or 4.  Removing
the text output and rerunning at 10x the work load to see if we could see any difference in 
performance of the lower arity levels produced the following (testing done on bowen):

 Arity =2 execution time : 29
 Arity =3 execution time : 23
 Arity =4 execution time : 23
 Arity =16 execution time : 32
 Arity =128 execution time : 127
 Arity =256 execution time : 217
setting Heap Arity = 512
Segmentation fault

This suggests that the cost of doing min searches within child groups is roughly balanced by 
the cost of reducing the max height of the tree for 2,3,4 and reduced access time for adjacent
data.  In the larger arity, the cost of doing the local calculation of minimum values 
greatly outweighs the tree traversal costs or any benefits arising from local data access.

sort3.cpp adjusted to look at arity {2,3,4,6,8,10,12,24}
 Arity =2 execution time : 9
 Arity =3 execution time : 7
 Arity =4 execution time : 6
 Arity =6 execution time : 7
 Arity =8 execution time : 8
 Arity =10 execution time : 7
 Arity =12 execution time : 8
 Arity =24 execution time : 11

Results suggests that for these tests, optimal arity is around 4.


