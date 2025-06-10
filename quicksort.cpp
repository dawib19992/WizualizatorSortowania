#include "quicksort.h"
#include <qnamespace.h>
#include <qobjectdefs.h>
#include <thread>

int QuickSort::selectMedian(std::vector<int>& data, int lewy, int prawy) {
    int mid = (lewy + prawy) / 2;
    int a = data[lewy], b = data[mid], c = data[prawy];

    // Znajdujemy medianę
    if ((a <= b && b <= c) || (c <= b && b <= a))
        return mid;
    if ((b <= a && a <= c) || (c <= a && a <= b))
        return lewy;
    return prawy;
}

void QuickSort::quickSort(std::vector<int>& data, int lewy, int prawy, std::mutex& mtx, bool& paused, bool& reset, MainWindow* window)
{
    if (prawy <= lewy || reset) return;

    int i = lewy - 1, j = prawy + 1;

    // Wybór pivota jako mediany z trzech elementów
    int pivotIndex = selectMedian(data, lewy, prawy);
    std::swap(data[pivotIndex], data[prawy]); // Przenosimy pivot na koniec
    float pivot = data[prawy];

    while (true)
    {
        do {
            i++;
            operations++;
            if (window) {
                QMetaObject::invokeMethod(window, [window, i, pivotIndex = prawy] {
                    window->drawData(i, pivotIndex);
                }, Qt::QueuedConnection);
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(window->getDelay()));
        } while (i < prawy && data[i] < pivot && !reset);

        do {
            j--;
            operations++;
            if (window) {
                QMetaObject::invokeMethod(window, [window, j, pivotIndex = prawy] {
                    window->drawData(j, pivotIndex);
                }, Qt::QueuedConnection);
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(window->getDelay()));
        } while (j > lewy && data[j] > pivot && !reset);

        if (reset) return;

        if (i <= j)
        {
            {
                std::lock_guard<std::mutex> lock(mtx);
                std::swap(data[i], data[j]);
                operations++;
            }

            if (window) {
                QMetaObject::invokeMethod(window, [window] {
                    window->drawData();
                }, Qt::QueuedConnection);
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(window->getDelay()));
        }
        else
        {
            break;
        }

        while (paused && !reset) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

    if (j > lewy)
        quickSort(data, lewy, j, mtx, paused, reset, window);
    if (i < prawy)
        quickSort(data, i, prawy, mtx, paused, reset, window);
}



void QuickSort::sort(std::vector<int>& data, std::mutex& mtx, bool& paused, bool& reset, MainWindow* window)
{
    operations = 0;
    int n = data.size();
    if (n <= 1) return;

    quickSort(data, 0, n - 1, mtx, paused, reset, window);
}

