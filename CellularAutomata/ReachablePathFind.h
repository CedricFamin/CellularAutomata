#pragma once

#include "IPathFinder.h"

class CellularAutomata;
class VisionCluster;

struct ReachablePathFindAdvancedProperty
{
    unsigned int dist;
    float walkable;
};

struct ReachablePathFindComparator
{
    inline bool operator()(Node<ReachablePathFindAdvancedProperty>* p1, Node<ReachablePathFindAdvancedProperty>* p2)
    {
        return (p1->GetData().dist + p1->GetDepth()) > (p2->GetData().dist + p2->GetDepth());
    }
};

class ReachablePathFind : public APathFinder<ReachablePathFindAdvancedProperty, ReachablePathFindComparator>
{
	typedef APathFinder<ReachablePathFindAdvancedProperty, ReachablePathFindComparator>::node_type node_type;
public:
	ReachablePathFind(CellularAutomata const * parCelluls, VisionCluster const * parVisionCluster);
	void Init(Position const & initialPos, Position const & dest);
	void SetInitialPos(Position const & parInitialPost) { _initialPos = parInitialPost; }

protected:
	virtual unsigned int EvalNode(node_type* node) const;
private:
	node_type * CreateGraph(int parX,  int parY, int dist, bool ignoreCollision);

	CellularAutomata const * FCelluls;
	VisionCluster const * FVisionCluster;
};

