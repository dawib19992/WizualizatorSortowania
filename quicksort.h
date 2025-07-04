#ifndef QUICKSORT_H
#define QUICKSORT_H

#include "AlgorytmSortowania.h"
#include "mainwindow.h"
class QuickSort : public SortingAlgorithm
{
private:
    Type type = Type::QuickSort;
    void quickSort(std::vector<int>& data, int lewy, int prawy, std::mutex& mtx, bool& paused, bool& reset, MainWindow* window);
    int selectMedian(std::vector<int>& data, int lewy, int prawy);

public:
    QuickSort() = default;
    void sort(std::vector<int>& data, std::mutex& mtx, bool& paused, bool& reset, MainWindow* window) override;
    Type getType() { return type;}
};

#endif // QUICKSORT_H
