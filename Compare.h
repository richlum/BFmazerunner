#ifndef _COMPARE_H
#define _COMPARE_H

#include "SquareMaze.h"

class SquareMazeNodeCompare {

 public:
/*
 * comparison operator that return true if left value is less than right value.
 */
  //bool operator() (const SquareMaze::SquareMazeNode * larg,
  //               const SquareMaze::SquareMazeNode * rarg) const
  bool operator() (const MazeNode * larg,
                   const MazeNode * rarg) const
    {  // here is the only place that we require SquareMazeNode methods.
	  return (nodeValue((SquareMaze::SquareMazeNode *)larg)-nodeValue(((SquareMaze::SquareMazeNode *) rarg))<0);
    }

/*
 * Given a MazeNode, evaluate its value based on modified Manhattan distance algorithm
 */
  int nodeValue(const SquareMaze::SquareMazeNode* aNode) const{
      int result = (abs(aNode->getX() - aNode->getExitX()) + abs(aNode->getY()-aNode->getExitY()))
                        * aNode->getMazeHeight() * aNode->getMazeWidth()
                        + aNode->getX()*aNode->getMazeHeight()+aNode->getY();
      return result;
  }

};

#endif
