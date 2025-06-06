#ifndef RADIXSORT_H
#define RADIXSORT_H

#include "AlgorytmSortowania.h"
#include "mainwindow.h"
class RadixSort : public SortingAlgorithm
{
protected:
    Type type = Type::RadixSort;
    void countingSort(std::vector<float>& data, int exp, std::mutex& mtx, bool& paused, bool& reset, MainWindow* window);
public:
    RadixSort() = default;
    void sort(std::vector<float>& data, std::mutex& mtx, bool& paused, bool& reset, MainWindow* window) override;
    Type getType() { return type;}
};

#endif // RADIXSORT_H
