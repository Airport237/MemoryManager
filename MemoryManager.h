#pragma once
#include <iostream>
#include <list>
#include <functional>
#include <stdint.h>
#include <cmath>
using namespace std;


class MemoryManager
{
    private:
        unsigned wordSize;
        std::function<int(int, void *)> allocator;
        uint8_t* storage = nullptr;
        size_t sizeInWords;
        int numHoles = 0;
        struct block
        {
            int start;
            int end;
            bool hole;
            block(int start, int end, bool hole)
            {
                this->start = start;
                this->end = end;
                this->hole = hole;
            };
        };
    std::list<block*> blocks;


    public:

        MemoryManager(unsigned wordSize, std::function<int(int, void *)> allocator)
        {
            this->wordSize = wordSize;
            this->allocator = allocator;
            storage = nullptr;
        };

        ~MemoryManager()
        {
            shutdown();
        };
        void initialize(size_t sizeInWords);
        void shutdown();
        void *allocate(size_t sizeInBytes);
        void free(void *address);
        void setAllocator(std::function<int(int, void *)> allocator);
        int dumpMemoryMap(char *filename);
        void *getList();
        void *getBitmap();
        unsigned getWordSize();
        void *getMemoryStart();
        unsigned getMemoryLimit();
        void printBlocks();
        int bestFit(int sizeInWords, void *list);
        int worstFit(int sizeInWords, void *list);
};