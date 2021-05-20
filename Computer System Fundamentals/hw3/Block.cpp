//
// Created by Brayden Archer on 10/23/20.
//

#include "Block.h"

Block::Block(bool valid, int tag, bool dirty, int bytes) { //Generic block constructor
    this->valid = valid;
    this->tag = tag;
    this->dirty = dirty;
    this->bytes = bytes;
}