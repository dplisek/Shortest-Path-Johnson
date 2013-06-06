//
//  t_edge.cpp
//  JarnikPrim
//
//  Created by Dominik Plíšek on 5/20/13.
//  Copyright (c) 2013 Dominik Plíšek. All rights reserved.
//

#include "t_edge.h"

t_edge::t_edge(int sourceIndex, int destinationIndex, int value) :
sourceIndex(sourceIndex), destinationIndex(destinationIndex), value(value) {
}

ostream& operator<<(ostream& os, const t_edge & edge) {
	os << "Edge: " << edge.sourceIndex << "-" << edge.destinationIndex << " ("
    << edge.value << ")";
	return os;
}