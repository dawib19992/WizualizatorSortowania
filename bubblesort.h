#ifndef BUBBLESORT_H
#define BUBBLESORT_H

#include "AlgorytmSortowania.h"
#include "mainwindow.h"
class BubbleSort : public SortingAlgorithm
{
protected:
    int operations = 0;
    Type type = Type::BubbleSort;
public:
    BubbleSort() = default;
    void sort(std::vector<float>& data, std::mutex& mtx, bool& paused, bool& reset, MainWindow* window);
    Type getType() { return type;}
};

#endif // BUBBLESORT_H
