#ifndef SLEEPSORT_H
#define SLEEPSORT_H

#include "AlgorytmSortowania.h"
#include "mainwindow.h"

class SleepSort : public SortingAlgorithm
{
public:
    void sort(std::vector<int>& data, std::mutex& mtx, bool& paused, bool& reset, MainWindow* window) override;
};

#endif // SLEEPSORT_H
