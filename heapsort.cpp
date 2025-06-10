#include "heapsort.h"
#include <thread>
#include <QMetaObject>

void heapify(std::vector<int>& data, int n, int i, std::mutex& mtx, bool& paused, bool& reset, MainWindow* window, int& operations) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && data[left] > data[largest]) largest = left;
    if (right < n && data[right] > data[largest]) largest = right;

    while (paused && !reset) std::this_thread::sleep_for(std::chrono::milliseconds(100));
    if (reset) return;

    mtx.lock();
    if (window) {
        QMetaObject::invokeMethod(window, [window, i, largest] {
            window->drawData(i, largest);
        }, Qt::QueuedConnection);
    }
    mtx.unlock();

    std::this_thread::sleep_for(std::chrono::milliseconds(window->getDelay()));

    if (largest != i) {
        std::swap(data[i], data[largest]);
        operations++;
        heapify(data, n, largest, mtx, paused, reset, window, operations);
    }
}

void HeapSort::sort(std::vector<int>& data, std::mutex& mtx, bool& paused, bool& reset, MainWindow* window) {
    operations = 0;
    int n = data.size();

    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(data, n, i, mtx, paused, reset, window, operations);
        if (reset) return;
    }

    for (int i = n - 1; i > 0; i--) {
        while (paused && !reset) std::this_thread::sleep_for(std::chrono::milliseconds(100));
        if (reset) return;

        mtx.lock();
        if (window) {
            QMetaObject::invokeMethod(window, [window, i] {
                window->drawData(0, i);
            }, Qt::QueuedConnection);
        }
        mtx.unlock();

        std::swap(data[0], data[i]);
        operations++;

        std::this_thread::sleep_for(std::chrono::milliseconds(window->getDelay()));
        heapify(data, i, 0, mtx, paused, reset, window, operations);
        if (reset) return;
    }
}
