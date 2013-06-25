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

void BFinitPaths(CGraph& g, int* h) {
    for (int i = 1; i <= g.getNodeCounter(); ++i) {
        h[i - 1] = INF_DIST;
    }
    h[g.getNodeCounter()] = 0;
}

void BFrelax(CGraph& g, int* h, int edgeIndex) {
    t_edge edge = g.getEdge(edgeIndex);
    int sourceDist = h[edge.sourceIndex - 1];
    if (sourceDist == INF_DIST) return;
    int destDist = h[edge.destinationIndex - 1];
    int newDestDist = sourceDist + edge.value;
    if (newDestDist >= destDist) return;
    h[edge.destinationIndex - 1] = newDestDist;
}

void BFrelaxAdditional(CGraph& g, int* h, int nodeIndex) {
    int sourceDist = h[g.getNodeCounter()];
    if (sourceDist == INF_DIST) return;
    int destDist = h[nodeIndex - 1];
    int newDestDist = sourceDist;
    if (newDestDist >= destDist) return;
    h[nodeIndex - 1] = newDestDist;
}

void BFrelaxAllEdges(CGraph& g, int* h) {
    for (int i = 1; i <= g.getEdgeCounter(); ++i) {
        BFrelax(g, h, i);
    }
    for (int i = 1; i <= g.getNodeCounter(); ++i) {
        BFrelaxAdditional(g, h, i);
    }
}

void relaxAllEdgesRepetetively(CGraph& g, int* h) {
    for (int i = 0; i < g.getNodeCounter(); ++i) {
        BFrelaxAllEdges(g, h);
    }
}

bool BFhasNegativeCycles(CGraph& g, int* h) {
    for (int i = 1; i <= g.getEdgeCounter(); ++i) {
        t_edge edge = g.getEdge(i);
        int sourceDist = h[edge.sourceIndex - 1];
        if (sourceDist == INF_DIST) continue;
        int destDist = h[edge.destinationIndex - 1];
        int newDestDist = sourceDist + edge.value;
        if (newDestDist < destDist) return true;
    }
    for (int i = 1; i <= g.getNodeCounter(); ++i) {
        int sourceDist = h[g.getNodeCounter()];
        if (sourceDist == INF_DIST) continue;
        int destDist = h[i - 1];
        int newDestDist = sourceDist;
        if (newDestDist < destDist) return true;
    }
    return false;
}

bool BF(CGraph& g, int* h) {
    BFinitPaths(g, h);
    relaxAllEdgesRepetetively(g, h);
    return !BFhasNegativeCycles(g, h);
}

void makeWeightsPositive(CGraph& g, int* h) {
    for (int i = 1; i <= g.getEdgeCounter(); i++) {
        t_edge e = g.getEdge(i);
        e.value = e.value + h[e.sourceIndex - 1] - h[e.destinationIndex - 1];
        g.setEdge(i, e);
    }
}

void DijskraInitPaths(CGraph& g, const int startIndex) {
    for (int i = 1; i <= g.getNodeCounter(); ++i) {
        t_node node = g.getNode(i);
        if (i == startIndex) {
            node.distance = 0;
        } else {
            node.distance = INF_DIST;
        }
        node.predecessorIndex = NO_NODE;
        g.setNode(i, node);
    }
}

int DijskraGetNearestIndex(CGraph& g, bool* processed) {
    int nearestDist = INF_DIST;
    int nearestIndex = NO_NODE;
    for (int i = 1; i <= g.getNodeCounter(); ++i) {
        if (processed[i - 1]) continue;
        if (g.getNode(i).distance < nearestDist) {
            nearestDist = g.getNode(i).distance;
            nearestIndex = i;
        }
    }
    return nearestIndex;
}

vector<int>* DijskraGetAdjEdges(CGraph& g, int nodeIndex) {
    vector<int>* adjEdges = new vector<int>();
    for (int i = 1; i <= g.getEdgeCounter(); ++i) {
        t_edge edge = g.getEdge(i);
		if (edge.sourceIndex != nodeIndex) continue;
        adjEdges->push_back(i);
	}
	return adjEdges;
}

void DijskraRelax(CGraph& g, int edgeIndex) {
    t_edge edge = g.getEdge(edgeIndex);
    int sourceDist = g.getNode(edge.sourceIndex).distance;
    if (sourceDist == INF_DIST) return;
    t_node destNode = g.getNode(edge.destinationIndex);
    int destDist = destNode.distance;
    int newDestDist = sourceDist + edge.value;
    if (newDestDist >= destDist) return;
    destNode.distance = newDestDist;
    destNode.predecessorIndex = edge.sourceIndex;
    g.setNode(edge.destinationIndex, destNode);
}

void Dijskra(CGraph& g, const int startIndex) {
    DijskraInitPaths(g, startIndex);
    bool* processed = new bool[g.getNodeCounter()];
    for (int i = 1; i <= g.getNodeCounter(); ++i) {
        processed[i - 1] = false;
    }
    int nearestIndex;
    while ((nearestIndex = DijskraGetNearestIndex(g, processed)) != NO_NODE) {
        processed[nearestIndex - 1] = true;
        vector<int>* adjEdges = DijskraGetAdjEdges(g, nearestIndex);
        for (vector<int>::iterator it = adjEdges->begin(); it != adjEdges->end(); ++it) {
            DijskraRelax(g, *it);
        }
        delete adjEdges;
    }
    delete [] processed;
}

int getRealDistance(CGraph& g, int* h, int i, int j) {
    if (g.getNode(j).distance == INF_DIST) return g.getNode(j).distance;
    return g.getNode(j).distance - h[i - 1] + h[j - 1];
}

void dijkstraFromEachNode(CGraph& g, int* h, int** distanceMatrix, int** predecessorIndexMatrix) {
    for (int i = 1; i <= g.getNodeCounter(); i++) {
        Dijskra(g, i);
        for (int j = 1; j <= g.getNodeCounter(); j++) {
            distanceMatrix[i - 1][j - 1] = getRealDistance(g, h, i, j);
            predecessorIndexMatrix[i - 1][j - 1] = g.getNode(j).predecessorIndex;
        }
    }
}

int** initMatrix(int size) {
    int** matrix = new int*[size];
    for (int i = 0; i < size; i++) {
        matrix[i] = new int[size];
    }
    return matrix;
}

bool Johnson(CGraph& g) {
    int* h = new int[g.getNodeCounter() + 1];
    if (!BF(g, h)) {
        delete [] h;
        return false;
    }
    makeWeightsPositive(g, h);
    int** distanceMatrix = initMatrix(g.getNodeCounter());
    int** predecessorIndexMatrix = initMatrix(g.getNodeCounter());
    dijkstraFromEachNode(g, h, distanceMatrix, predecessorIndexMatrix);
    g.setMatrix(distanceMatrix, predecessorIndexMatrix);
    delete [] h;
    for (int i = 0; i < g.getNodeCounter(); i++) {
        delete [] distanceMatrix[i];
        delete [] predecessorIndexMatrix[i];
    }
    delete [] distanceMatrix;
    delete [] predecessorIndexMatrix;
    return true;
}

#ifndef __PROGTEST__

int main(int argc, const char * argv[])
{
    CGraph *g = new CGraph(12, 12, true);
    
    t_node node;
    for (int i = 1; i <= 12; i++) {
        g->setNode(i, node);
    }
    
    t_edge edge2(1, 5, 4);
    t_edge edge3(1, 6, 12);
    t_edge edge5(1, 7, 11);
    t_edge edge6(1, 11, -1);
    t_edge edge8(2, 10, 1);
    t_edge edge9(3, 4, 3);
    t_edge edge11(5, 10, -1);
    t_edge edge12(6, 9, 3);
    t_edge edge13(7, 1, 11);
    t_edge edge14(8, 6, -2);
    t_edge edge15(9, 1, -5);
    t_edge edge16(9, 2, 12);
    
    g->setEdge(1, edge2);
    g->setEdge(2, edge3);
    g->setEdge(3, edge5);
    g->setEdge(4, edge6);
    g->setEdge(5, edge8);
    g->setEdge(6, edge9);
    g->setEdge(7, edge11);
    g->setEdge(8, edge12);
    g->setEdge(9, edge13);
    g->setEdge(10, edge14);
    g->setEdge(11, edge15);
    g->setEdge(12, edge16);
    
    Johnson(*g);
    
    cout << *g << endl;
    
    delete g;
    return 0;
}

#endif