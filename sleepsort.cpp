#include "sleepsort.h"
#include <thread>
#include <chrono>
#include <QMetaObject>
#include "mainwindow.h"
#include <algorithm>
#include <vector>

void SleepSort::sort(std::vector<int>& data, std::mutex& mtx, bool& paused, bool& reset, MainWindow* window)
{
    operations = 0;
    if (data.size() <= 1) return;

    std::vector<int> result;
    std::vector<std::thread> threads;

    int minValue = *std::min_element(data.begin(), data.end());
    int offset = 0;
    if (minValue < 0) {
        offset = -minValue;
    }

    for (int value : data)
    {
        threads.emplace_back([&, value] {
            while (paused && !reset) std::this_thread::sleep_for(std::chrono::milliseconds(100));
            if (reset) return;

            std::this_thread::sleep_for(std::chrono::milliseconds((value + offset) * 10));

            if (reset) return;

            {
                std::lock_guard<std::mutex> lock(mtx);
                result.push_back(value);
                operations++;
            }

            if (window)
            {
                QMetaObject::invokeMethod(window, [window, &result] {
                    window->drawData();
                }, Qt::QueuedConnection);
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(window->getDelay()));
        });
    }

    for (auto& t : threads) {
        if (t.joinable()) t.join();
    }

    if (!reset)
    {
        std::lock_guard<std::mutex> lock(mtx);
        data = result;

        if (window)
        {
            QMetaObject::invokeMethod(window, [window] {
                window->drawData();
            }, Qt::QueuedConnection);
        }
    }
}
