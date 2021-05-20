//
// Created by Brayden Archer on 10/23/20.
//

#include "Set.h"

Set::Set(std::vector<Block> blocks, int index) { //Set constructor
    this->blocks = std::move(blocks); //Takes vector of blocks and moves into class
    this->index = index;
}

