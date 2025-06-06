#include "radixsort.h"
#include <thread>
#include <chrono>
#include <cmath>
#include <QMetaObject>

void RadixSort::countingSort(std::vector<float>& data, int exp, std::mutex& mtx, bool& paused, bool& reset, MainWindow* window) {
    int n = data.size();
    std::vector<float> output(n);
    std::vector<int> count(10, 0);

    for (int i = 0; i < n; i++) {
        if (reset) return;
        while (paused && !reset) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        int digit = static_cast<int>(std::floor(data[i]) / exp) % 10;
        count[digit]++;
        operations++;
        if (window) {
            QMetaObject::invokeMethod(window, [window, i] {
                window->drawData(i, -1);
            }, Qt::QueuedConnection);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(window->getDelay()));
    }

    for (int i = 1; i < 10; i++) {
        count[i] += count[i - 1];
    }
    for (int i = n - 1; i >= 0; i--) {
        if (reset) return;
        while (paused && !reset) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        int digit = static_cast<int>(std::floor(data[i]) / exp) % 10;
        output[count[digit] - 1] = data[i];
        count[digit]--;
        operations++;
        if (window) {
            QMetaObject::invokeMethod(window, [window, i, count, digit] {
                window->drawData(i, count[digit]);
            }, Qt::QueuedConnection);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(window->getDelay()));
    }
    mtx.lock();
    for (int i = 0; i < n; i++) {
        data[i] = output[i];
    }
    mtx.unlock();
    if (window) {
        QMetaObject::invokeMethod(window, [window] {
            window->drawData();
        }, Qt::QueuedConnection);
    }
}

void RadixSort::sort(std::vector<float>& data, std::mutex& mtx, bool& paused, bool& reset, MainWindow* window) {
    operations = 0;
    int n = data.size();
    if (n <= 1) return;
    float max = data[0];
    for (int i = 1; i < n; i++) {
        if (data[i] > max) max = data[i];
    }
    int maxInt = static_cast<int>(std::floor(max));

    for (int exp = 1; maxInt / exp > 0; exp *= 10) {
        if (reset) return;
        countingSort(data, exp, mtx, paused, reset, window);
    }
    if (window) {
        QMetaObject::invokeMethod(window, [window] {
            window->drawData();
        }, Qt::QueuedConnection);
    }
}
