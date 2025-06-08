#ifndef BUBBLESORT_H
#define BUBBLESORT_H

#include "AlgorytmSortowania.h"
#include "mainwindow.h"
class BubbleSort : public SortingAlgorithm
{
protected:
    Type type = Type::BubbleSort;
public:
    BubbleSort() = default;
    void sort(std::vector<int>& data, std::mutex& mtx, bool& paused, bool& reset, MainWindow* window) override;
    Type getType() { return type;}
};

#endif // BUBBLESORT_H
