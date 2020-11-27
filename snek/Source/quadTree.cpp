#include "quadTree.h"

QuadTree::QuadTree(int level, int* bounds) {
    this->level = level;
    for(int i=0;i<4;i++)
        this->bounds[i] = bounds[i];
    
}

void QuadTree::clear() {
    objects.clear();
    nodes.clear();
}


