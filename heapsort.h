#ifndef HEAPSORT_H
#define HEAPSORT_H

#include "AlgorytmSortowania.h"
#include "mainwindow.h"
class HeapSort : public SortingAlgorithm
{
private:
    Type type = Type::HeapSort;

public:
    HeapSort() = default;
    void sort(std::vector<int>& data, std::mutex& mtx, bool& paused, bool& reset, MainWindow* window);
    Type getType() { return type;}
};

#endif // HEAPSORT_H
