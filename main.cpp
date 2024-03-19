#include "MemoryManager.h"

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
int main()
{
    MemoryManager mem = MemoryManager(4, bestFit);
    mem.initialize(16);
    mem.printBlocks();




    return 0;
}