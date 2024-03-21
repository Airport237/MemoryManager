#include "MemoryManager.h"

int bestFit(int sizeInWords, void *list)
{
    uint16_t* listU = (uint16_t*)list;
    int minInd = -1;
    int min = 65536;
    for (int i = 1; i <= listU[0]; i = i+2)
    {
        //if the length of the hole is less than min and greater than or equal 
        //to the sizeInWords set it to the new min
        if (listU[i+1] >= sizeInWords && (listU[i+1] < min))
        {
            min = listU[i+1];
            minInd = listU[i];
        }
    }
    //cout << minInd << endl;
    return minInd;
};
int main()
{
    MemoryManager mem = MemoryManager(4, bestFit);
    mem.initialize(1024);
    //mem.printBlocks();
    void* q = mem.allocate(4);
    void* p = mem.allocate(25);
    mem.allocate(30);
    //mem.allocate(45);
    mem.free(p);
    mem.free(q);

    mem.printBlocks();


    return 0;
}