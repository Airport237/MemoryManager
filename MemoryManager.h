#pragma once
#include <iostream>
#include <functional>

class MemoryManager
{
    private:
        unsigned wordSize;
        std::function<int(int, void *)> allocator;
        uint8_t* storage = nullptr;
        size_t sizeInWordsG;
    public:
        struct hole
        {
            int start;
            int end;
            hole(int start_, int end_)
            {
                start = start_;
                end = end_;
            };
        };
        
        std::list<hole> holes;
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