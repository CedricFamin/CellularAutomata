#include "ReachablePathFind.h"

#include "CellularAutomata.h"
#include "SMAHeat.h"
#include "ReachabilityCluster.h"

ReachablePathFind::ReachablePathFind(CellularAutomata const * parCelluls, ReachabilityCluster const * parReachabilityCluster)
	: FCelluls(parCelluls)
	, FReachabilityCluster(parReachabilityCluster)
{
}


ReachablePathFind::node_type * ReachablePathFind::CreateGraph(int parX,  int parY, int dist, bool ignoreCollision)
{
    struct Params
    {
        Params(int px, int py, node_type * _parent = 0)
        {
            x = px;
            y = py;
            parent = _parent;
        }
        int x;
        int y;
        node_type * parent;
    };
    
    std::list<Params> stackParam;
    stackParam.push_back(Params(parX, parY));
    
    while (stackParam.size())
    {
        int x = stackParam.front().x;
        int y = stackParam.front().y;
        Params params = stackParam.front();
        stackParam.pop_front();
        
		if ((x >= FReachabilityCluster->MaxX() || y >= FReachabilityCluster->MaxY() || x < FReachabilityCluster->MinX() || y < FReachabilityCluster->MinY()))
        {
            continue;
        }
        
        if (this->_edgeMap[y][x] != 0)
        {
            if (params.parent)
                params.parent->AddNode(this->_edgeMap[y][x]);
            continue;
        }
        
        
        unsigned int computedDist = 0;
        {
            computedDist = abs(_destination.x - x) + abs(_destination.y - y);
        }
        
        node_type* edge = new node_type();
        if (params.parent)
            params.parent->AddNode(edge);
        this->_edgeMap[y][x] = edge;
        edge->GetData().dist = computedDist;
		edge->GetData().walkable = !((*FCelluls)[y][x].IsWall) || ignoreCollision;
		if (ignoreCollision && !(*FCelluls)[y][x].IsWall)
			ignoreCollision = false;
        edge->X() = x;
        edge->Y() = y;
        
        std::pair<int, int> moves[4] = {
            std::make_pair( 1, 0),
            std::make_pair(-1, 0),
            std::make_pair( 0, 1),
            std::make_pair( 0,-1)
        };
        
        for (std::pair<int, int> & move : moves)
        {
            Params param(x + move.first, y + move.second, edge);
            stackParam.push_back(param);
        }
    }
    
    return NULL;
}

void ReachablePathFind::Init(Position const & initialPos, Position const & dest)
{
	_initialPos = initialPos;
	_destination = dest;
	this->_edgeMap.Init(FCelluls->GetCoordConverter().GetX(), FCelluls->GetCoordConverter().GetY());
	CreateGraph(dest.x, dest.y, 0, true);
}

unsigned int ReachablePathFind::EvalNode(node_type* node) const
{
	 if (!node->GetData().walkable)
        return (unsigned int)-1;
    return node->GetData().dist + node->GetDepth();
}