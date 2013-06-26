#include <assert.h>

#include "LinkPathFinder.h"

#include "Link.h"

LinkPathFind * LinkPathFind::instanceBigHackFunThis = NULL;

LinkPathFind::LinkPathFind()
	: FDist(0)
{
	instanceBigHackFunThis = this;
}

void LinkPathFind::Init(std::vector<Link const*> parLinks, int worldX, int worldY)
{
	this->_edgeMap.Init(worldX, worldY);
	for (auto link : parLinks)
	{
		node_type * node = new node_type();
		node->GetData().walkable = true;
		node->GetData().link = link;
		node->X() = link->x;
		node->Y() = link->y;
		assert(this->_edgeMap[link->y][link->x] == NULL);
		this->_edgeMap[link->y][link->x] = node;
	};

	for (auto link : parLinks)
	{
		node_type * node = this->_edgeMap[link->y][link->x];
		assert(node);
		for (auto connectedLinkWithDist : link->ReachableLinks)
		{
			Link const * currentLink = connectedLinkWithDist.link;
			assert(currentLink);
			assert(this->_edgeMap[currentLink->y][currentLink->x]);
			node->AddNode(this->_edgeMap[currentLink->y][currentLink->x]);
		}
	};
}

void LinkPathFind::ResultPath(node_type * parNode)
{
	FDist = 0;
	node_type * currentNode = parNode;
	while (currentNode)
	{
		if (currentNode->GetParent())
		{
			for (auto & reachableLink : currentNode->GetData().link->ReachableLinks)
			{
				if (reachableLink.link == currentNode->GetParent()->GetData().link)
				{
					FDist += reachableLink.dist;
					break;
				}
			}
		}
		currentNode = currentNode->GetParent();
	}
}

void LinkPathFind::EvalDist(node_type* node) const
{
	Link const * currentLink = node->GetData().link;
	unsigned int dist = UINT_MAX;

	for (auto destination : FDestinations)
	{
		unsigned int distance = std::sqrtl(std::powl(currentLink->x - destination->x, 2) + std::powl(currentLink->y - destination->y, 2));
		dist = std::min(dist, distance);
	}
	node->GetData().dist = dist;

}

void LinkPathFind::SetInitialPos(Link const * parLink)
{
	_initialPos.x = parLink->x;
	_initialPos.y = parLink->y;
}

unsigned int LinkPathFind::EvalNode(node_type* node) const
{
	return node->GetData().dist + node->GetDepth();
}



LinkPathFind::node_type * LinkPathFind::CreateGraph(int parX,  int parY, int dist, bool ignoreCollision)
{
	return NULL;
}

void LinkPathFind::SetDestinations(std::list<Link const*> const & parDestinations)
{
	FDestinations = parDestinations;
	for (unsigned int i = 0; i < _edgeMap.height(); ++i)
	{
		for (unsigned int j = 0; j < _edgeMap.height(); ++j)
		{
			if (_edgeMap[i][j])
			EvalDist(_edgeMap[i][j]);
		}
	}
}