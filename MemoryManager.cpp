#include "MemoryManager.h"

void MemoryManager::initialize(size_t sizeInWords)
{
    if (sizeInWords <= 65536)
    {
        sizeInWordsG = sizeInWords;
        //Creates the memory array
        storage = new uint8_t[sizeInWords*getWordSize()];
        //Initializes first hole as entire array 
        holes.push_back(hole(0,sizeInWords));
    }
}

MemoryManager::~MemoryManager()
{
    shutdown();
}

unsigned MemoryManager::getWordSize()
{
    return wordSize;
}

void MemoryManager::shutdown()
{
    if (storage != nullptr)
    {
        //Needs to delete ll nodes and potentially free get list stuff
        delete[] storage;
    }
}

void* MemoryManager::getList()
{
    //As long as a memory array has been intiliazed and some memory has been allocated into the linked list such that
    //the holes list is not in its intial state
    if (storage != nullptr && !(holes.front().start == 0 && holes.front().end == sizeInWordsG))
    {
        uint16_t length = holes.size();
        uint16_t* list = new uint16_t[(2*length + 1)];
        list[0] = length;
        int counter = 1;
        for (auto iter = holes.begin(); iter != holes.end(); ++iter)
        {
            list[counter] = iter->start;
            counter++;
            list[counter] = (iter->end - iter->start);
            counter++;
        }
        return list;
    }
}


void* MemoryManager::allocate(size_t sizeInBytes)
{
    void* list = getList();
    int holeInd = allocator(sizeInBytes*wordSize, list);
    


    delete[] list;
}




int bestFit(int sizeInWords, void *list)
{
    uint16_t* listU = (uint16_t*)list;
    int minInd = -1;
    int min = -1;
    for (int i = 1; i < listU[0]; i = i+2)
    {
        //if the length of the hole is less than min (and its not first big enough hole) and greater than or equal 
        //to the sizeInWords set it to the new min
        if (listU[i+1] >= sizeInWords && (min == -1 || listU[i+1] < min))
        {
            min = listU[i+1];
            minInd = listU[i];
        }
    }
    return minInd;
};

int worstFit(int sizeInWords, void *list)
{
    uint16_t* listU = (uint16_t*)list;
    int minInd = -1;
    int min = -1;
    for (int i = 1; i < listU[0]; i = i+2)
    {
        //if the length of the hole is greater than min and greater than or equal 
        //to the sizeInWords set it to the new min
        if (listU[i+1] >= sizeInWords && listU[i+1] > min)
        {
            min = listU[i+1];
            minInd = listU[i];
        }
    }
    return minInd;
};