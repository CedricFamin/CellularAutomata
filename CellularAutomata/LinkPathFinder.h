#pragma once

#include <list>

#include "IPathFinder.h"

class Link;

struct LinkPathFindAdvancedProperty
{
    unsigned int dist;
	Link const * link;
	bool walkable;
};

struct LinkPathFindComparator
{
    inline bool operator()(Node<LinkPathFindAdvancedProperty>* p1, Node<LinkPathFindAdvancedProperty>* p2)
    {
        return (p1->GetData().dist) > (p2->GetData().dist);
    }
};

class LinkPathFind : public APathFinder<LinkPathFindAdvancedProperty, LinkPathFindComparator>
{
	typedef APathFinder<LinkPathFindAdvancedProperty, LinkPathFindComparator>::node_type node_type;
public:
	LinkPathFind();
	void Init(std::vector<Link const*> parInitialLinks, int worldX, int worldY);
	void SetInitialPos(Link const * parLink);
	void SetDestinations(std::list<Link const*> const & parDestinations);
	virtual void ResultPath(node_type * parNode);
	unsigned int GetDist() const { return FDist; }

	static LinkPathFind * instanceBigHackFunThis;
	void EvalDist(node_type* node) const;
	bool HasARechableTarget() const;
protected:
	virtual unsigned int EvalNode(node_type* node) const;
private:
	node_type * CreateGraph(int parX,  int parY, int dist, bool ignoreCollision);
	unsigned int FDist;
	std::list<Link const*> FDestinations;
	
};
