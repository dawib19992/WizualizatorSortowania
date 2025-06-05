#ifndef ALGORYTMSORTOWANIA_H
#define ALGORYTMSORTOWANIA_H
#include <vector>
#include <mutex>

enum Type{
    MergeSort,
};
class SortingAlgorithm {
protected:
    int operations = 0;
public:
    virtual ~SortingAlgorithm() = default;
    virtual void sort(std::vector<int>& data, std::mutex& mtx, bool& paused, bool& reset) = 0;
    int getOperations() const { return operations; }
};


#endif // ALGORYTMSORTOWANIA_H
