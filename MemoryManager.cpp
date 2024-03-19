#include "MemoryManager.h"

void MemoryManager::initialize(size_t sizeInWords)
{
    if (sizeInWords <= 65536)
    {
        this->sizeInWords = sizeInWords;
        //Creates the memory array
        storage = new uint8_t[sizeInWords*getWordSize()];
        //Initializes first hole as entire array 
        blocks.push_back(new block(0,sizeInWords, true));
        numHoles = 1;
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
    if (storage != nullptr && !(blocks.front()->start == 0 && blocks.front()->end == sizeInWords))
    {
        uint16_t length = blocks.size();
        uint16_t* list = new uint16_t[(2*length + 1)];
        list[0] = length;
        int counter = 1;
        for (auto iter = blocks.begin(); iter != blocks.end(); iter++)
        {
            list[counter] = (*iter)->start;
            counter++;
            list[counter] = ((*iter)->end - (*iter)->start);
            counter++;
        }
        return list;
    }
}


void* MemoryManager::allocate(size_t sizeInBytes)
{
    void* list = getList();
    int blockLength = ceil((float)sizeInBytes/wordSize);
    int holeInd = allocator(blockLength, list);
    delete[] list;

    if (holeInd == -1)
    {
        return nullptr;
    }
    //Find the block
    auto iter = blocks.begin();
    for (iter; iter != blocks.end(); iter++)
    {
        if ((*iter)->start == holeInd)
            break;
    }
    //Could not find hole
    block* current = *iter;
    if (iter == blocks.end())
        return nullptr;

    
    //Need to find start and end of new hole
    int newStart = holeInd + blockLength;
    int newEnd = current->end;

    //Update the changing hole to a memblock
    current->end = current->start + blockLength;
    current->hole = false;
    numHoles--;

    //If the new block doesn't fit perfectly we need to append a hole to the end
    if (newEnd - newStart != 0)
    {
        iter++;
        blocks.insert(iter,new  block(newStart, newEnd, true));
        numHoles++;
    }
    return &storage[holeInd*wordSize];
}

void MemoryManager::free(void *address)
{
    
}



void MemoryManager::setAllocator(std::function<int(int, void *)> allocator)
{
    this->allocator = allocator;
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

