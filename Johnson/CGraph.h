//
//  CGraph.h
//  JarnikPrim
//
//  Created by Dominik Plíšek on 5/20/13.
//  Copyright (c) 2013 Dominik Plíšek. All rights reserved.
//

#ifndef __JarnikPrim__CGraph__
#define __JarnikPrim__CGraph__

#include "t_edge.h"
#include "t_node.h"
#include <vector>

class CGraph {
    vector<t_node> nodes;
	vector<t_edge> edges;
    int** distanceMatrix;
    int** predecessorIndexMatrix;
public:
    int getEdgeCounter() const;
    t_edge& getEdge(const int index) const;
    void setEdge(const int index, const t_edge& edge);
    void addEdge(const t_edge& edge);
    void deleteEdge(const int index);
    int getNodeCounter() const;
    t_node& getNode(const int index) const;
    void setNode(const int index, const t_node& node);
    void addNode(const t_node& node);
    friend ostream& operator<<(ostream& os, const CGraph & graph);
    void setMatrix(int** distanceMatrix, int** predecessorIndexMatrix);
};

#endif /* defined(__JarnikPrim__CGraph__) */
