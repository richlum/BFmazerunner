/*
 * BFMazeRunner.cpp
 *
 * Heavily based on RandomMazeRunner. Primary changes to pickNextNode. Some functionality
 * moved from solveMaze down into pickNextNode where it can better co-ordinate with the
 * update of node information such as visitation status.
 *
 * Also uses ExtraNodeInfo as in RandomMazeRunner but does not update during exploration
 * since in this case, prioritization given by the modified Manhattan distance algorithm
 * does not guarantee that the next node to be explored is physically adjacent to the
 * last.  As such prior assumptions about building solution paths based on order of
 * exploration no longer apply.  Instead, the solution path is built after we have
 * reached the exit node by looking at visited nodes from exit back to start node.
 *
 *
 *  Created on: 2011-02-09
 *      Author: rlum
 */


#include <iostream>
#include <cassert>
#include <cstdlib>
#include <ctime>

#include "Maze.h"
#include "MazeRunner.h"

#include "BFMazeRunner.h"
#include "UnsLLPQueue.h"
#include "Compare.h"
#include "SquareMaze.h"
#include "Maze.h"
#include "DHeap.h"
#include "PQueue.h"

BFMazeRunner::BFMazeRunner() {
}


BFMazeRunner::BFMazeRunner(PriorityQueue<MazeNode *,SquareMazeNodeCompare > * pQ){
    this->priorityQ = pQ; //pointer assignment
}

BFMazeRunner::~BFMazeRunner() {
}
/*
 * solve the maze using supplied priority queue and comparator
 * @pre: all Nodes must have VisitationStatus=NotVisited
 * @pre: all Nodes should have no ExtraNodeInfo
 * @post: all Nodes in solution path from startNode to exitNode will have
 * 		ExtraNodeInfo->nextInSolution updated to provide a solution path for printing
 */
void BFMazeRunner::solveMaze (Maze *maze,ostream& solutionOutput)
{
    MazeNode *currNode;

    currNode=maze->getStartMazeNode();
    while (!currNode->isExitNode()) {
	//* for debugging
	/*
	   currNode->print(cerr);
	   cerr << ", visit state = " << currNode->getVisitationState() << ", num neigh = " << currNode->getNumNeighbors() ;
	   cerr << "\n";
	 */
	MazeNode *nextNode;
	nextNode=pickNextNode(currNode,priorityQ);
	currNode=nextNode;
	if (nextNode==NULL){
		cout << "NO SOLUTION"<< endl;  // priority queue return a null, empty queue and we havent solved maze.
		return;
		}
    }
    // Maze Solved, here curNode = exitNode.  Update all neighbours to point to me
    // and recursively get their Visited neighbours to point to them.

    // running without visualizer option works fine

    //visualizer seems to be writting to ExtraNodeInfo and interfering
    // with our use of it for solution path calculation.
    // wipe them all out and initialize to null.

    deleteExtraNodeInfo(maze);
    MazeNodeIterator* mit = maze->getAllMazeNodes();

    //initialize all nodes to have null ExtraNodeInfo
    while(mit->hasNext()){
	MazeNode* mazeNode = mit->next();
	//mazeNode->print(cout);
	//cout << "getinfo " << mazeNode->getInfo();
	//ExtraNodeInfo * x = 
	getExtraNodeInfo(mazeNode);
	//x->nextInSolution=NULL;
	mazeNode->setInfo(NULL);
	// this one seems to have cleared the visualizer induced problem
	// nextInSolution will be seen as NULL (required for pointToMe)
    }

    //now that we have reset all extraNodeInfo
    //we can use it to capture our solution.
    pointToMe(currNode,maze->getStartMazeNode(), currNode);

    // print out our solution
    solutionOutput << "BEST FIRST\n"; // print out the type of solution
    currNode=maze->getStartMazeNode();
    while (!currNode->isExitNode()) {
	// print this node in the solution
	currNode->print(solutionOutput);
	// get to the next node
	ExtraNodeInfo *extraInfo;
	extraInfo=getExtraNodeInfo(currNode);
	currNode=extraInfo->nextInSolution;
    }
    // and the exit node
    currNode->print(solutionOutput);
    solutionOutput << "\n"; // it's on one line

    // delete all ExtraNodeInfo's
    deleteExtraNodeInfo(maze);
}
/**
 * most methods taken directly from
 * RandomMazeRunner
 */
void BFMazeRunner::deleteExtraNodeInfo(Maze *maze)
{
    MazeNodeIterator *iter;
    iter=maze->getAllMazeNodes();
    while (iter->hasNext()) {
	ExtraNodeInfo *extraInfo;
	extraInfo=(ExtraNodeInfo *)(iter->next()->getInfo());
	delete extraInfo;
    }
    delete iter;
}
BFMazeRunner::ExtraNodeInfo *BFMazeRunner::getExtraNodeInfo(MazeNode *currNode)
{
    ExtraNodeInfo *extraInfo;
    extraInfo=(ExtraNodeInfo *)(currNode->getInfo());
    if (extraInfo==NULL) {
	extraInfo=new ExtraNodeInfo;
	currNode->setInfo(extraInfo);
    }
    return extraInfo;
}

/*
 * Unique to BFMazeRunner, pickNextNode takes a given node as input,
 * finds all its neighbours and inserts them into priorityQueue and
 * return the highest prioirty.  It also udates the visitStatus of 
 * each node.
 *  @return the next node to explore according to the supplied priority queue and
 *  	comparator
 *  @param currNode - the currentNode to explore.  This method interrogates all the
 *  	neighbours and puts all neighbours into the priority queue.
 */
MazeNode *BFMazeRunner::pickNextNode(MazeNode *currNode, PriorityQueue<MazeNode *,SquareMazeNodeCompare >* pQ)
{
    MazeNode *nextNode;
    if (currNode->getVisitationState()!=MazeNode::Visited){
	pQ->insert(currNode);
	currNode->setVisitationState(MazeNode::Visited);
	MazeNodeIterator* mzIt = currNode->getNeighbors();
	while (mzIt->hasNext()){
	    MazeNode* neighbour = mzIt->next();
	    if (neighbour->getVisitationState()!=MazeNode::Visited){
		pQ->insert(neighbour	);
		neighbour->setVisitationState(MazeNode::VisitInProgress);
	    }
	}
    }
    if (!pQ->isEmpty()){
    	nextNode = pQ->findMin(); //get min from queue
    	pQ->deleteMin();  // remove min from queue
    	return nextNode;
    }else{
    	return NULL;
    }
}

/*
 * utility method used during debugging to test when two nodes are neighbours
 * @return true if two nodes supplied are reachable maze neighbours
 */

bool BFMazeRunner::neigbours(MazeNode* lnode, MazeNode* rnode){
    MazeNodeIterator* mzIt =lnode->getNeighbors();
    while (mzIt->hasNext()){
	MazeNode* neighbour = mzIt->next();
	if (neighbour==rnode){
	    return true;
	}
    }
    cout << "not neighbours case " ;
    lnode->print(cout);
    rnode->print(cout);
    cout << endl;
    return false;
}


/*
 * pointToMe expects the exit node as input and will recursively 
 * get all neighbours who have been visited and update their
 * nextInSolution to point towards the calling node.
 * @pre : all Nodes must either have no ExtraNodeInfo or must
 * have the contents of ExtraNodeInfo->nextInSolution set to NULL
 * @return false  if we are unable to update any neighbours to point to
 * supplied node.
 * @param aNode to examing for neighbours that have been visited that should
 * 		point to aNode.
 * @param startNode for a reference point to know when we have reached the origin
 * 		in our reverse exploration of visited nodes.
 * @param exitNode for reference point to address interworking bug with visualizer
 * 		mode of runmaze.  pointToMe requires all nodes in path have NULL or no
 * 		reference in ExtraNodeInfo->nextInSolution in order to update.  Without
 * 		explictly testing for exitNode, visualizer has neighbours set to non null
 * 		value that we have to overwrite in order to build solution path.
 		The requirement for this parameter is likely deprecated with the 
		initialization of ExtraNodeInfo before calling this method but
		this code is left here for saftey since we havent actually located
		the source of the code in the visualizer that is interfering.
 *
 * base cases
 * 	neighbour is start node -> set him to point to us, return true
 * 	no visited neighbours without nextInSolution free -> return false
 * recursive case on our immediate neighbours who don't already have
 * 		nextInSolution datafilled. Make those point to supplied aNode.
 * 		Each iteration depends on neighbour nodes having
 * 		nextInSolution datafilled to reduce size of each subsequent
 		recursive call.
 *
 */
bool BFMazeRunner::pointToMe(MazeNode* aNode,MazeNode* startNode,MazeNode* exitNode){
    MazeNodeIterator* mzIt =aNode->getNeighbors();
    while (mzIt->hasNext()){
	MazeNode* neighbour = mzIt->next();

	if (neighbour->getVisitationState()==MazeNode::Visited){
	    ExtraNodeInfo *extraInfo;
	    extraInfo = getExtraNodeInfo(neighbour);

	    // only update if we havent already been here
	    // this is how we reduce the workset on each
	    // recursive call.
	    if ((extraInfo->nextInSolution == NULL)||
		    (aNode==exitNode))
		// inserted to override visualiser setting on nodes adjacent to
		// exit.  MayNot be required since subsequently added re-initialization
		// of all ExtraNodeInfo.
		// Unable to find source of change between completion of
		// maze and this location.
	    {
		extraInfo->nextInSolution=aNode;
		/*   // debug info
		     cout << "set " ;
		     neighbour->print(cout);
		     cout << " nextinSolution set to " ;
		     aNode->print(cout);
		     cout << endl;
		 */
		if (neighbour == startNode){
		    return true; // stop when entry node is found
		}else{
		    // recursively look at neighbours to point at me.
		    // from set of neighbours that we updated.
		    pointToMe(neighbour,startNode,exitNode);
		}
	    }
	}// finished looking at this neighbour and descendants
    }// next neighbour
    //	cout << "pointToMe found a deadend " << endl;
    return false; // failed to find available neighbour to point to us
    		  // done with this search path. Not a part of solution path.

}
