#ifndef MERGESORT_H
#define MERGESORT_H
#include "AlgorytmSortowania.h"
#include "mainwindow.h"

class MergeSort : public SortingAlgorithm
{
private:
    Type type = Type::MergeSort;
    void merge(std::vector<float>& data, int left, int mid, int right, std::mutex& mtx);
public:
    MergeSort() = default;
    void sort(std::vector<float>& data, std::mutex& mtx, bool& paused, bool& reset, MainWindow* window) override;
    Type getType() { return type;}
};

#endif // MERGESORT_H
