//
// Created by Brayden Archer on 10/23/20.
//
#include <vector>

#ifndef HW3_BLOCK_H
#define HW3_BLOCK_H

class Block {
public:
    bool valid;
    int tag;
    bool dirty;
    int bytes;
    Block(bool valid, int tag, bool dirty, int bytes);
};

#endif //HW3_BLOCK_H
