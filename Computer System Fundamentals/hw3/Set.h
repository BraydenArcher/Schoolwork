//
// Created by Brayden Archer on 10/23/20.
//
#include "Block.h"
#include <vector>

#ifndef HW3_SET_H
#define HW3_SET_H


class Set {
public:
    int index;
    std::vector<Block> blocks;
    Set(std::vector<Block> blocks, int index);
};
#endif //HW3_SET_H
