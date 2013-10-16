/*
 * BFMazeRunner.h
 *
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
 *  Created on: 2011-02-09
 *      Author: rlum
 */



#ifndef BFMAZERUNNER_H_
#define BFMAZERUNNER_H_

#include "PQueue.h"
#include "UnsLLPQueue.h"
#include "Compare.h"
#include "DHeap.h"

using namespace std;


class BFMazeRunner : public MazeRunner {
    protected:
	struct ExtraNodeInfo {
	    MazeNode *nextInSolution;
	};

	MazeNode *pickNextNode(MazeNode *currNode, PriorityQueue<MazeNode *,SquareMazeNodeCompare >* priorityQ);
	ExtraNodeInfo *getExtraNodeInfo(MazeNode *currNode);
	void deleteExtraNodeInfo(Maze *maze);

    public:
	BFMazeRunner();
	BFMazeRunner(PriorityQueue<MazeNode *,SquareMazeNodeCompare >   *);
	~BFMazeRunner();

	void solveMaze (Maze *maze, ostream& solutionOutput);

    private:
	PriorityQueue<MazeNode*, SquareMazeNodeCompare>* priorityQ; 
	SquareMazeNodeCompare* squareMazeNodeComparator;

	// true if given nodes are reachable mazenode neighbours
	bool neigbours(MazeNode* , MazeNode* ); 

	// search for neighbours with extraNodeinfo to point to us.
	bool pointToMe(MazeNode* ,MazeNode*,MazeNode* ); //endnode, startnode,exitNode
};

#endif /* BFMAZERUNNER_H_ */
