#include "quicksort.h"
#include <qnamespace.h>
#include <qobjectdefs.h>
#include <thread>

void QuickSort::quickSort(std::vector<float>& data, int lewy, int prawy,
                          std::mutex& mtx, bool& paused, bool& reset, MainWindow* window)
{
    if (prawy <= lewy || reset) return;

    int i = lewy - 1, j = prawy + 1;
    float pivot = data[(lewy + prawy) / 2];

    while (true)
    {
        // Szukanie z lewej strony
        do {
            i++;
            if (window) {
                QMetaObject::invokeMethod(window, [window, i, pivotIndex = (lewy + prawy) / 2] {
                    window->drawData(i, pivotIndex);
                }, Qt::QueuedConnection);
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(window ? window->getDelay() : 50));
        } while (i < prawy && data[i] < pivot && !reset);

        // Szukanie z prawej strony
        do {
            j--;
            if (window) {
                QMetaObject::invokeMethod(window, [window, j, pivotIndex = (lewy + prawy) / 2] {
                    window->drawData(j, pivotIndex);
                }, Qt::QueuedConnection);
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(window ? window->getDelay() : 50));
        } while (j > lewy && data[j] > pivot && !reset);

        if (reset) return;

        if (i <= j)
        {
            {
                std::lock_guard<std::mutex> lock(mtx);
                std::swap(data[i], data[j]);
            }

            if (window) {
                QMetaObject::invokeMethod(window, [window] {
                    window->drawData(); // odśwież po zamianie
                }, Qt::QueuedConnection);
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(window ? window->getDelay() : 50));
        }
        else
        {
            break;
        }

        while (paused && !reset) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

    quickSort(data, lewy, j, mtx, paused, reset, window);
    quickSort(data, i, prawy, mtx, paused, reset, window);
}



void QuickSort::sort(std::vector<float>& data, std::mutex& mtx,
                     bool& paused, bool& reset, MainWindow* window)
{
    operations = 0;
    int n = data.size();
    if (n <= 1) return;

    quickSort(data, 0, n - 1, mtx, paused, reset, window);
}

