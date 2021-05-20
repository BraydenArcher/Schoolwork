//
// Created by Brayden Archer on 10/23/20.
//

#include "Cache.h"
#include <string>
#include <iostream>
#include <utility>
#include "Block.h"
#include "Set.h"
#include <cmath>

//Constructor that instantiates every variable
Cache::Cache (int sets, int blocks, int bytes, bool allocate, bool write, bool type, std::vector<char> processorStatus, std::vector<std::string> memoryAddress) {
    this->sets = sets;
    this->blocks = blocks;
    this->bytes = bytes;
    this->allocate = allocate;
    this->write = write;
    this->type = type;
    this->processorStatus = std::move(processorStatus);
    this->memoryAddress = std::move(memoryAddress);
    totalLoads = 0;
    totalStores = 0;
    loadHits = 0;
    loadMisses = 0;
    storeHits = 0;
    storeMisses = 0;
    totalCycles = 0;
    for(int i = 0; i < sets; i++) { //Fill all of the vectors with empty shells of blocks/sets
        std::vector<Block> blockList;
        for(int j = 0; j < blocks; j++) {
            Block tempBlock(false, -1, false, bytes); //Create empty block
            blockList.emplace_back(tempBlock); //put it in da back
        }
        Set tempSet(blockList, i); //New temp set
        setList.emplace_back(tempSet); //Add temp set to cache vector
    }
}

void Cache::print() { //Just prints stats for last simulation run
    this->totalStores = storeHits + storeMisses;
    this->totalLoads = loadHits + loadMisses;
    std:: cout << "Total loads: " << totalLoads << std::endl;
    std:: cout << "Total stores: " << totalStores << std::endl;
    std:: cout << "Load hits: " << loadHits << std::endl;
    std:: cout << "Load misses: " << loadMisses << std::endl;
    std:: cout << "Store hits: " << storeHits << std::endl;
    std:: cout << "Store misses: " << storeMisses << std::endl;
    std:: cout << "Total cycles: " << totalCycles << std::endl;
}

void Cache::simulate() { //Main simulation method, checks if each block is a hit and then acts accordingly
    for(int i = 0; (unsigned long) i < memoryAddress.size(); i++) {
        if(processorStatus[i] == 'l') {
            if(checkIfHit(memoryAddress[i])) { //If cache hit for load
                loadHit(memoryAddress[i]);
            } else { //If cache miss for load
                loadMiss(memoryAddress[i]);
            }
        } else {
            if(checkIfHit(memoryAddress[i])) {//Cache hit for store
                storeHit(memoryAddress[i]);
            } else { //Cache miss for store
                storeMiss(memoryAddress[i]);
            }
        }
    }
}

void Cache::LRU(std::string address) { //This method moves whatever trace its given to the back to ensure that it is the most recently used
    std::string addressCopy = std::move(address);
    int index = getIndex(addressCopy);
    int tag = getTag(addressCopy);
    for(int i = 0; (unsigned long) i < setList[index].blocks.size(); i++) {
        if(setList[index].blocks[i].tag == tag) {
            Block tempBlock = setList[index].blocks[i]; //Create temp copy of the block
            setList[index].blocks.erase(setList[index].blocks.begin() + i); //delete the block
            setList[index].blocks.emplace_back(tempBlock); //read the block at the end
            return; //Exit
        }
    }
}

void Cache::loadHit(std::string address) { //Load hit method
    if(type) { //If its LRU do that
        LRU(std::move(address));
    }
    loadHits++; //Increase load hits and cycles
    totalCycles++;
}

void Cache::loadMiss(std::string address) { //Load miss method
    std::string addressCopy = std::move(address); //Make local copy of string
    loadMisses++; //Increase load misses
    if(!fullCache(getIndex(addressCopy))) { //Checks if cache is full
        addBlock(addressCopy); //If not add block
    } else {
        replaceBlock(addressCopy); //Otherwise replace
    }
    totalCycles += 100 * bytes/4; //Cycles increment for writing to memory
}

void Cache::storeHit(std::string address) {//store hit method
    if(type) { //If LRU rearrange
        LRU(address);
    }
    storeHits++; //Increase cycles/ store hits
    totalCycles++;
    if(write) { //Write 100 cycles if write through
        totalCycles += 100;
    } else { //Else its a write back, mark as dirty and add one cycle
        markBlockAsDirty(std::move(address));
        totalCycles++;
    }
}

void Cache::storeMiss(std::string &address) { //Store miss method
    storeMisses++; //Increment store misses
    if(allocate) { //If write-allocate
        totalCycles += 100 * bytes/4; //Write to memory
        if(!fullCache(getIndex(address))) { //If cache is full add or replace depending on result
            addBlock(address);
        } else {
            replaceBlock(address);
        }
    }
    if(write) { //If its a write through
        totalCycles += 100; //Add 100 cycles
    } else {
        markBlockAsDirty(address); //Otherwise mark as dirty and move on
    }
}

void Cache::markBlockAsDirty(std::string address) { //Mark as dirty method, searches the blocks list in the given set and sets the dirty bool to true
    std::string addressCopy = std::move(address);
    int index = getIndex(addressCopy);
    int tag = getTag(addressCopy);
    std::vector<Block> blockList = setList[index].blocks;
    for(int i = 0; (unsigned long) i < blockList.size(); i++) {
        if(blockList[i].tag == tag) {
            blockList[i].dirty = true;
            setList[index].blocks = blockList;
            return;
        }
    }
}

void Cache::addBlock(std::string address) { //Adds a block to the cache
    std::string addressCopy = std::move(address); //Local copy of string
    totalCycles++; //Cycles increment
    int index = getIndex(addressCopy); //get index
    int tag = getTag(addressCopy); //get tag
    Block newBlock(true, tag, false, bytes); //Create a new block with given info
    setList[index].blocks.emplace_back(newBlock); //Put block in the back
    /*std::vector<Block> blockList = setList[index].blocks;
    for(int i = 0; (unsigned long) i < blockList.size() - 1; i++) {
        blockList[i + 1] = blockList[i];
    }
    blockList[0] = newBlock;
    setList[index].blocks = blockList;*/
}

void Cache::replaceBlock(std::string address) {//Replaces block
    std::string addressCopy = std::move(address); //Local copy of string
    totalCycles++; //Cycles increment
    int index = getIndex(addressCopy); //Get index of trace
    int tag = getTag(addressCopy); //get tag of trace
    Block newBlock(true, tag, false, bytes);//Create a new block
    if(setList[index].blocks.back().dirty) { //If its dirty write to mem
        totalCycles += 100 * bytes/4; //Cycles increment
    }
    setList[index].blocks.erase(setList[index].blocks.begin()); //Erase first block
    setList[index].blocks.emplace_back(newBlock); //Add new block at back
}


bool Cache::fullCache(int index) {
    return setList[index].blocks.size() == (unsigned long) blocks; //Checks if cache is full by comparing size to the cache size
}

uint32_t Cache::getTag(std::string address) const { //returns the tag of given trace
    std::string addressCopy = std::move(address); //Local string copy
    uint32_t indexBits = log2(sets); //gets the amount of bits for index
    uint32_t offsetBits = log2(bytes); //gets the amount of bits for offset
    uint32_t addressInBase10 = stol("0x" + addressCopy, nullptr ,16); //Get trace in base 10
    return addressInBase10 >> (indexBits + offsetBits); //Right shift to delete right bits to get tag
}

uint32_t Cache::getIndex(std::string address) const { //Returns the index of given trace
    std::string addressCopy = std::move(address); //Local string copy
    uint32_t indexBits = log2(sets); //gets the amount of bits for index
    uint32_t offsetBits = log2(bytes); //gets the amount of bits for offset
    uint32_t addressInBase10 = stol("0x" + addressCopy, nullptr ,16); //Get trace in base 10
    return (getBitsFromMtoN(offsetBits - 1, offsetBits+indexBits - 1) & addressInBase10) >> offsetBits; //Uses mask to get rid of left most and right most bits, then bit shift
}

uint32_t Cache::getBitsFromMtoN(uint32_t a, uint32_t b) { //Creates a mask for a bits to b bits of a number
    //But the specified bits in the input when bitwise and with a number
    uint64_t mask = 0;
    for (uint64_t i=a; i<=b; i++) {
        mask |= 1 << i;
    }
    return mask; //Returns calculated mask
}

bool Cache::checkIfHit(std::string address) { //Checks if given trace is a hit
    std::string addressCopy = std::move(address); //Local copy of string
    int index = getIndex(addressCopy); //Get index of trace
    int tag = getTag(addressCopy); //get tag of trace
    std::vector<Block> blockList = setList[index].blocks; //Local copy of blockList for convenience
    for(int i = 0; (unsigned long) i < blockList.size(); i++) { //check every block in set
        if(blockList[i].tag == tag) { //If tag is the same, hit! Return true
            return true;
        }
    }
    return false; //No tags, no dice, miss :( return false
}

