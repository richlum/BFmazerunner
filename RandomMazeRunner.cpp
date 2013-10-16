#include <iostream>
#include <cassert>
#include <cstdlib>
#include <ctime>

#include "Maze.h"
#include "MazeRunner.h"
#include "RandomMazeRunner.h"

RandomMazeRunner::RandomMazeRunner ()
{
  // Call the static class initializer. It will run only the first
  // time it's called.
  initClass();
}
RandomMazeRunner::~RandomMazeRunner ()
{
}
void RandomMazeRunner::solveMaze (Maze *maze,ostream& solutionOutput)
{
        MazeNode *currNode;

        currNode=maze->getStartMazeNode();
        while (!currNode->isExitNode()) {

                /* for debugging
                currNode->print(cerr);
                cerr << "\n";
                */

                // retrieve (or create) the extra info for this node
                ExtraNodeInfo *extraInfo;
                extraInfo=getExtraNodeInfo(currNode);

                currNode->setVisitationState(MazeNode::VisitInProgress);

                // pick a next node
                MazeNode *nextNode;
                nextNode=pickNextNode(currNode);
                currNode->setVisitationState(MazeNode::Visited);

                // and go to that node
                extraInfo->nextInSolution=nextNode;
                currNode=nextNode;
        }

        // yaay!!!  we solved it!

        // print out our solution
        solutionOutput << "RANDOM\n"; // print out the type of solution
        currNode=maze->getStartMazeNode();
        while (!currNode->isExitNode()) {

                // print this node in the solution
                currNode->print(solutionOutput);
                solutionOutput << " "; // separate by spaces

                // get to the next node
                ExtraNodeInfo *extraInfo;
                extraInfo=getExtraNodeInfo(currNode);
                currNode=extraInfo->nextInSolution;
        }
        // and the exit node
        currNode->print(solutionOutput);
        solutionOutput << " "; // separate by spaces
        solutionOutput << "\n"; // it's on one line


        // delete all ExtraNodeInfo's
        deleteExtraNodeInfo(maze);
}
void RandomMazeRunner::deleteExtraNodeInfo(Maze *maze)
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
RandomMazeRunner::ExtraNodeInfo *RandomMazeRunner::getExtraNodeInfo(MazeNode *currNode)
{
        ExtraNodeInfo *extraInfo;
        extraInfo=(ExtraNodeInfo *)(currNode->getInfo());
        if (extraInfo==NULL) {
                extraInfo=new ExtraNodeInfo;
                currNode->setInfo(extraInfo);
        }
        return extraInfo;
}
MazeNode *RandomMazeRunner::pickNextNode(MazeNode *currNode)
{
        MazeNode *nextNode;
        int numNeighbors,pickNum,r;
        numNeighbors=currNode->getNumNeighbors();
	r=rand();
        pickNum=(int)((double)(numNeighbors)*(double)(rand())/((double)(RAND_MAX)+1.0));
        assert(pickNum>=0 && pickNum<numNeighbors);

        // now get the node corresponding to that number
        MazeNodeIterator *iter;
        int i;
        iter=currNode->getNeighbors();
        for (i=0; i<=pickNum; i++) {
                assert(iter->hasNext());
                nextNode=iter->next();
        }
        delete iter;

        return nextNode;
}

void RandomMazeRunner::initClass()
{
  static bool called = false;

  if (!called) {
    // Ensure that this function is called only once per program
    called = true;

#ifdef DEBUG
    // Initialize the randomizer to a constant for testing purposes.
    // Zero is a pretty, round constant.
    srand(0);
#else
    // Initialize the randomizer with time (which changes pretty frequently).
    srand(time(NULL));
#endif
  }
}
