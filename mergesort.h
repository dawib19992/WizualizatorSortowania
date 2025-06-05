#ifndef MERGESORT_H
#define MERGESORT_H
#include "AlgorytmSortowania.h"

class MergeSort : public SortingAlgorithm
{
private:
    Type type = Type::MergeSort;
    void merge(std::vector<int>& data, int left, int mid, int right, std::mutex& mtx);
    void mergeSort(std::vector<int>& data, int left, int right, std::mutex& mtx, bool& paused, bool& reset);
public:
    MergeSort();
    void sort(std::vector<int>& data, std::mutex& mtx, bool& paused, bool& reset) override;
    Type getType() { return type;}
};

#endif // MERGESORT_H
