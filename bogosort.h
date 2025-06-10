#ifndef BOGOSORT_H
#define BOGOSORT_H

#include "AlgorytmSortowania.h"
#include <random>

class BogoSort : public SortingAlgorithm
{
public:
    void sort(std::vector<int>& data, std::mutex& mtx, bool& paused, bool& reset, MainWindow* window) override;

private:
    bool isSorted(const std::vector<int>& data);
    void shuffle(std::vector<int>& data);
};

#endif // BOGOSORT_H
