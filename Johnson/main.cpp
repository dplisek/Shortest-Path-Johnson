//
//  main.cpp
//  Johnson
//
//  Created by Dominik Plíšek on 5/31/13.
//  Copyright (c) 2013 Dominik Plíšek. All rights reserved.
//

#include <iostream>

#ifndef __PROGTEST__
#include "CGraph.h"
#endif

#define INF_DIST 9999
#define NO_NODE 0

class CGraphWrapper {
    vector<t_node> additionalNodes;
	vector<t_edge> additionalEdges;
    CGraph* g;
public:
    CGraphWrapper(CGraph *g);
    int getEdgeCounter() const;
    t_edge& getEdge(const int index) const;
    void addEdge(const t_edge& edge);
    void addAdditionalEdge(const t_edge& edge);
    int getNodeCounter() const;
    t_node& getNode(const int index) const;
    void setNode(const int index, const t_node& node);
    void addNode(const t_node& node);
    void addAdditionalNodeAndJoinItWithAllOtherNodesUsingZeroWeightEdges(const t_node& node);
    void setMatrix(int** distanceMatrix, int** predecessorIndexMatrix);
};

CGraphWrapper::CGraphWrapper(CGraph *g) : g(g) {
}

int CGraphWrapper::getEdgeCounter() const {
    return g->getEdgeCounter() + (int) additionalEdges.size();
}

t_edge& CGraphWrapper::getEdge(const int index) const {
    if (index <= g->getEdgeCounter()) {
        return g->getEdge(index);
    }
	return *(new t_edge(additionalEdges[index - g->getEdgeCounter() - 1]));
}

void CGraphWrapper::addEdge(const t_edge& edge) {
    g->addEdge(edge);
}

int CGraphWrapper::getNodeCounter() const {
	return g->getNodeCounter() + (int) additionalNodes.size();
}

t_node& CGraphWrapper::getNode(const int index) const {
    if (index <= g->getNodeCounter()) {
        return g->getNode(index);
    }
	return *(new t_node(additionalNodes[index - g->getNodeCounter() - 1]));
}

void CGraphWrapper::setNode(const int index, const t_node &node) {
    if (index <= g->getNodeCounter()) {
        g->setNode(index, node);
        return;
    }
    additionalNodes[index - g->getNodeCounter() - 1] = node;
}

void CGraphWrapper::addNode(const t_node& node) {
    g->addNode(node);
}

void CGraphWrapper::addAdditionalNodeAndJoinItWithAllOtherNodesUsingZeroWeightEdges(const t_node &node) {
    additionalNodes.push_back(node);
    int newNodeIndex = getNodeCounter();
    for (int i = 1; i < newNodeIndex; i++) {
        t_edge edge(newNodeIndex, i, 0);
        addAdditionalEdge(edge);
    }
}

void CGraphWrapper::addAdditionalEdge(const t_edge &edge) {
    additionalEdges.push_back(edge);
}

void CGraphWrapper::setMatrix(int** distanceMatrix, int** predecessorIndexMatrix) {
    g->setMatrix(distanceMatrix, predecessorIndexMatrix);
}



void initPaths(CGraphWrapper *wrapper, const int startIndex) {
    for (int i = 1; i <= wrapper->getNodeCounter(); ++i) {
        t_node node = wrapper->getNode(i);
        if (i == startIndex) {
            node.distance = 0;
        } else {
            node.distance = INF_DIST;
        }
        node.predecessorIndex = NO_NODE;
        wrapper->setNode(i, node);
    }
}

void relax(CGraphWrapper *wrapper, int edgeIndex) {
    t_edge edge = wrapper->getEdge(edgeIndex);
    int sourceDist = wrapper->getNode(edge.sourceIndex).distance;
    if (sourceDist == INF_DIST) return;
    t_node destNode = wrapper->getNode(edge.destinationIndex);
    int destDist = destNode.distance;
    int newDestDist = sourceDist + edge.value;
    if (newDestDist >= destDist) return;
    destNode.distance = newDestDist;
    destNode.predecessorIndex = edge.sourceIndex;
    wrapper->setNode(edge.destinationIndex, destNode);
}

void relaxAllEdges(CGraphWrapper *wrapper) {
    for (int i = 1; i <= wrapper->getEdgeCounter(); ++i) {
        relax(wrapper, i);
    }
}

void relaxAllEdgesRepetetively(CGraphWrapper *wrapper) {
    for (int i = 0; i < wrapper->getNodeCounter() - 1; ++i) {
        relaxAllEdges(wrapper);
    }
}

bool hasNegativeCycles(CGraphWrapper *wrapper) {
    for (int i = 1; i <= wrapper->getEdgeCounter(); ++i) {
        t_edge edge = wrapper->getEdge(i);
        int sourceDist = wrapper->getNode(edge.sourceIndex).distance;
        if (sourceDist == INF_DIST) continue;
        t_node destNode = wrapper->getNode(edge.destinationIndex);
        int destDist = destNode.distance;
        int newDestDist = sourceDist + edge.value;
        if (newDestDist < destDist) return true;
    }
    return false;
}

bool BF(CGraphWrapper *wrapper, const int startIndex) {
    initPaths(wrapper, startIndex);
    relaxAllEdgesRepetetively(wrapper);
    return !hasNegativeCycles(wrapper);
}

void addExtraNodeToWrapper(CGraphWrapper *wrapper) {
    t_node node;
    wrapper->addAdditionalNodeAndJoinItWithAllOtherNodesUsingZeroWeightEdges(node);
}

bool Johnson(CGraph& g) {
    CGraphWrapper *wrapper = new CGraphWrapper(&g);
    addExtraNodeToWrapper(wrapper);
    if (!BF(wrapper, wrapper->getNodeCounter())) {
        return false;
    }
    
    return true;
}

#ifndef __PROGTEST__

int main(int argc, const char * argv[])
{
    CGraph *g = new CGraph;
    
    t_node node1;
    t_node node2;
    t_node node3;
    t_node node4;
    t_node node5;
    
    g->addNode(node1);
    g->addNode(node2);
    g->addNode(node3);
    g->addNode(node4);
    g->addNode(node5);
    
    t_edge edge2(1, 2, 3);
    t_edge edge3(1, 3, 6);
    t_edge edge5(2, 3, 2);
    t_edge edge6(2, 4, 6);
    t_edge edge8(3, 4, -3);
    t_edge edge9(4, 1, 1);
    t_edge edge11(4, 5, 1);
    t_edge edge12(5, 1, -2);
    t_edge edge13(5, 2, 3);
    t_edge edge14(5, 3, 4);
    
    g->addEdge(edge2);
    g->addEdge(edge3);
    g->addEdge(edge5);
    g->addEdge(edge6);
    g->addEdge(edge8);
    g->addEdge(edge9);
    g->addEdge(edge11);
    g->addEdge(edge12);
    g->addEdge(edge13);
    g->addEdge(edge14);
    
    Johnson(*g);
    
    cout << *g << endl;
    
    delete g;
    return 0;
}

#endif