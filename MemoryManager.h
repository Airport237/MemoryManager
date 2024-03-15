#include <iostream>
#include <functional>
using namespace std;

class MemoryManager
{
    public:
        MemoryManager(unsigned wordSize, std::function<int(int, void *)> allocator);
};