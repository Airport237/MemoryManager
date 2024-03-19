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
            if((*iter)->hole)
            {
                list[counter] = (*iter)->start;
                counter++;
                list[counter] = ((*iter)->end - (*iter)->start);
                counter++;
            }
        }
        return list;
    }
    return nullptr;
}


void* MemoryManager::allocate(size_t sizeInBytes)
{
    void* list = getList();
    int blockLength = std::ceil((float)sizeInBytes/wordSize);
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
    //convert address from bytes to blocks
    int blockInd = ((uint8_t*)address - storage)/wordSize;

    //Find the block
    auto iter = blocks.begin();
    for(iter; iter != blocks.end(); iter++)\
    {
        if ((*iter)->start == blockInd)
            break;
    }
    block* current = *iter;

    //If you cant find the block
    if (iter == blocks.end() || current->hole == true)
        return;
    
    //Turn block to hole
    current->hole = true;
    numHoles++;

    //3 cases, Block left hole right, block right and hole left, and hole left and hole right (end and start are the same as if a block was to the right or left respectivley)
    auto prev = iter;
    bool start = true;
    auto next = iter;
    bool end = true;
    //To prevent seg faults, if we can move forward or backward we know we are not at the end/start
    if(prev != blocks.begin())
    {
        prev--;
        start = false;
    }

    if (next != blocks.end())
    {
        next++;
        end = false;
    }

    if(!start && (*prev)->hole && !end && (*next)->hole)
    {
        block* prevBlock = *prev;
        block* nextBlock = *next;
        current->start = prevBlock->start;
        current->end = nextBlock->end;
        blocks.erase(prev);
        blocks.erase(next);
        delete prevBlock;
        delete nextBlock;
    }else if((start  || (!start && !(*prev)->hole) && (*next)->hole))
    {
        //Block is at the start or there is a bloack to the left and there is a hole to the right merge into big hole 
        // (If there is not a hole this simply becomes a hole with the same start and end and we dont have to do anything)
        block* nextBlock = *next;
        current->end = nextBlock->end;
        blocks.erase(next);
        delete nextBlock;
        
    }else if (end || (!end && !(*next)->hole) && (*prev)->hole)
    {
        //Same as above case but at the end of the list
        block* prevBlock = *prev;
        current->start = prevBlock->start;
        blocks.erase(prev);
        delete prevBlock;
    }

}



void MemoryManager::setAllocator(std::function<int(int, void *)> allocator)
{
    this->allocator = allocator;
}

void MemoryManager::printBlocks() {
    for (auto iter = blocks.begin(); iter != blocks.end(); iter++) 
    {
        std::cout << "Start: " << (*iter)->start << std::endl;
        std::cout << "End: " << (*iter)->end << std::endl;
        std::cout << "Hole: " << (*iter)->hole << std::endl;
        std::cout << std::endl;
    }
}

// int bestFit(int sizeInWords, void *list)
// {
//     uint16_t* listU = (uint16_t*)list;
//     int minInd = -1;
//     int min = -1;
//     for (int i = 1; i < listU[0]; i = i+2)
//     {
//         //if the length of the hole is less than min (and its not first big enough hole) and greater than or equal 
//         //to the sizeInWords set it to the new min
//         if (listU[i+1] >= sizeInWords && (min == -1 || listU[i+1] < min))
//         {
//             min = listU[i+1];
//             minInd = listU[i];
//         }
//     }
//     return minInd;
// };

// int worstFit(int sizeInWords, void *list)
// {
//     uint16_t* listU = (uint16_t*)list;
//     int minInd = -1;
//     int min = -1;
//     for (int i = 1; i < listU[0]; i = i+2)
//     {
//         //if the length of the hole is greater than min and greater than or equal 
//         //to the sizeInWords set it to the new min
//         if (listU[i+1] >= sizeInWords && listU[i+1] > min)
//         {
//             min = listU[i+1];
//             minInd = listU[i];
//         }
//     }
//     return minInd;
// };

