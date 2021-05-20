//
// Created by Brayden Archer on 10/23/20.
//
#include "Set.h"
#include <vector>
#include <string>

#ifndef HW3_CACHE_H
#define HW3_CACHE_H

class Cache {
public:
    int sets;
    int blocks;
    int bytes;
    bool allocate;
    bool write;
    bool type;
    std::vector<char> processorStatus;
    std::vector<std::string> memoryAddress;
    std::vector<Set> setList;
    long totalLoads;
    long totalStores;
    long loadHits;
    long loadMisses;
    long storeHits;
    long storeMisses;
    long totalCycles;
    Cache(int sets, int blocks, int bytes, bool allocate, bool write, bool type, std::vector<char> processorStatus, std::vector<std::string> memoryAddress);
    void simulate();
    void print();
private:
    bool checkIfHit(std::string address); //
    uint32_t getTag(std::string address) const;
    uint32_t getIndex(std::string address) const;
    static uint32_t getBitsFromMtoN(uint32_t a, uint32_t b);
    void markBlockAsDirty(std::string address);
    bool fullCache(int index);
    void addBlock(std::string address);
    void replaceBlock(std::string address);
    void loadHit(std::string address);
    void loadMiss(std::string address);
    void storeHit(std::string address);
    void storeMiss(std::string &address);
    void LRU(std::string address);
};


#endif //HW3_CACHE_H
