
#include "bogosort.h"
#include <thread>
#include <chrono>
#include <QMetaObject>
#include "mainwindow.h"

bool BogoSort::isSorted(const std::vector<int>& data)
{
    for (size_t i = 0; i + 1 < data.size(); ++i)
    {
        if (data[i] > data[i + 1])
            return false;
    }
    return true;
}

void BogoSort::shuffle(std::vector<int>& data)
{
    static std::random_device rd;
    static std::mt19937 g(rd());
    std::shuffle(data.begin(), data.end(), g);
}

void BogoSort::sort(std::vector<int>& data, std::mutex& mtx, bool& paused, bool& reset, MainWindow* window)
{
    operations = 0;
    if (data.size() <= 1) return;

    while (!reset && !isSorted(data))
    {
        while (paused && !reset)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        if (reset) return;

        {
            std::lock_guard<std::mutex> lock(mtx);
            shuffle(data);
            operations++;
        }

        if (window)
        {
            QMetaObject::invokeMethod(window, [window] {
                window->drawData();
            }, Qt::QueuedConnection);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(window->getDelay()));
    }
}
