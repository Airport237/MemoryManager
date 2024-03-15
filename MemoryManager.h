#pragma once
#include <iostream>
#include <functional>
using namespace std;

class MemoryManager
{
    public:
        MemoryManager(unsigned wordSize, std::function<int(int, void *)> allocator);
        ~MemoryManager();
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


};