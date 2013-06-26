//
//  IPathFinder.h
//  DynamicPathFinding
//
//  Created by Cédric Famin on 30/03/13.
//
//

#ifndef DynamicPathFinding_IPathFinder_h
#define DynamicPathFinding_IPathFinder_h

#include <list>
#include <map>
#include <set>
#include <algorithm>
#include <queue>
#include <iostream>

#include "Node.h"


struct Position
{
    int x;
    int y;
};


class IPathFinder
{
public:
    
    virtual ~IPathFinder() { }
    
    //virtual void Init(Map const * map) = 0;
    virtual unsigned int ComputePath() = 0;
    virtual void ClearPathAndDestination() = 0;
};

// Provide a simple A* algorithm
// Create the graph and fill the EdgeMap in the init method
// Implement your evaluation func in eval_node
template<typename Data, typename NodeComparator>
class APathFinder : public IPathFinder
{
public:
    typedef Data data_type;
    typedef Node<data_type> node_type;
    typedef std::priority_queue<node_type*, std::vector<node_type*>, NodeComparator> OpenList;
    typedef std::set<node_type*> ClosedList;
    typedef std::list<node_type*> NodeList;
    
    struct EdgeMap
    {
        void Init(unsigned int width, unsigned int height)
        {
            this->_width = width;
            this->_height = height;
            this->_container = new node_type*[width * height];
            for (unsigned int i = 0; i < width * height; ++i)
            {
                this->_container[i] = 0;
            }
        }
        
        node_type ** operator[](unsigned int y)
        {
            return _container + y * this->_width;
        }
        
        node_type ** operator[](unsigned int y) const
        {
            return _container + y * this->_width;
        }
        
        unsigned int height() const { return this->_height; }
        unsigned int width() const  { return this->_width; }
    private:
        
        unsigned int _width;
        unsigned int _height;
        
        node_type **  _container;
    };
    
    APathFinder() { }
    virtual ~APathFinder() { }
    
    virtual unsigned int ComputePath()
    {
        OpenList openList;
        ClosedList closedList;
        node_type* currentNode = 0;
        
        this->_root = this->_edgeMap[_initialPos.y][_initialPos.x];
        if (!this->_root)
            return false;
        
        openList.push(this->_root);
        this->_root->Open(true);
		this->_root->SetDepth(0);
        while (!openList.empty())
        {
            currentNode = openList.top();
            openList.pop();
            currentNode->Close(true);
            currentNode->Open(false);
            closedList.insert(currentNode);
            
            if (this->EvalNode(currentNode) == currentNode->GetDepth())
                break;
            
            for (node_type * node : currentNode->Edges())
            {
                if (!node->GetData().walkable && !node->Closed())
                {
                    node->Close(true);
                    closedList.insert(node);
                }

                if (!node->Closed() && !node->Opened())
                {
                    node->SetParent(currentNode);
                    node->SetDepth(currentNode->GetDepth() + 1);
                    openList.push(node);
                    node->Open(true);
                }
                
                if (node->Closed() && node->GetDepth() > currentNode->GetDepth() + 1)
                {
                    node->Close(false);
                    node->Open(true);
                    node->SetParent(currentNode);
                    closedList.erase(node);
                    openList.push(node);
                }
            }

            currentNode = 0;
        }
		unsigned int dist = 0;
        if (currentNode)
        {
			dist = this->EvalNode(currentNode);
            currentNode->Open(false);
            currentNode->Close(false);
        }

		this->ResultPath(currentNode);

		//std::cout << "Closed list size : " << closedList.size() << std::endl;

        for (node_type * node : closedList)
        {
            node->Reinit();
        }
        
        while (openList.size() > 0)
        {
            openList.top()->Reinit();
            openList.pop();
        }
        
        return dist;
    }
	virtual void ResultPath(node_type * parNode) {}
    
    virtual void ClearPathAndDestination()
    {
        
    }
    
protected:
    virtual unsigned int EvalNode(node_type* node) const = 0;
    Position   _initialPos;
    Position   _destination;
    node_type* _root;
    EdgeMap    _edgeMap;
    OpenList   _openList;
    ClosedList _closedList;
};

#endif
