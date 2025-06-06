#include "mergesort.h"
#include <qnamespace.h>
#include <qobjectdefs.h>
#include <thread>
#include <chrono>

void MergeSort::merge(std::vector<float>& data, int left, int mid, int right, std::mutex& mtx) {
    std::vector<int> temp(right - left + 1);
    int i = left;
    int j = mid + 1;
    int k = 0;

    while (i <= mid && j <= right) {
        if (data[i] <= data[j]) {
            temp[k] = data[i];
            i++;
        } else {
            temp[k] = data[j];
            j++;
        }
        k++;
        operations++;
    }

    while (i <= mid) {
        temp[k] = data[i];
        i++;
        k++;
    }

    while (j <= right) {
        temp[k] = data[j];
        j++;
        k++;
    }

    for (int p = 0; p < k; p++) {
        data[left + p] = temp[p];
    }
}

void MergeSort::sort(std::vector<float>& data, std::mutex& mtx, bool& paused, bool& reset, MainWindow* window) {
    operations = 0;
    int n = data.size();
    if (n <= 1) return;

    for (int size = 1; size < n; size *= 2) {
        for (int left = 0; left < n - size; left += 2 * size) {
            while (paused && !reset) {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
            if (reset) return;

            int mid = left + size - 1;
            int right = std::min(left + 2 * size - 1, n - 1);
            mtx.lock();
            std::vector<int> temp(right - left + 1);
            int i = left, j = mid + 1, k = 0;
            while (i <= mid && j <= right) {
                if (window) {
                    QMetaObject::invokeMethod(window, [window, i, j] {
                        window->drawData(i,j);
                    }, Qt::QueuedConnection);
                }
                if (data[i] <= data[j]) {
                    temp[k++] = data[i++];
                } else {
                    temp[k++] = data[j++];
                }
                operations++;
                std::this_thread::sleep_for(std::chrono::milliseconds(window->getDelay()));
            }
            while (i <= mid) temp[k++] = data[i++];
            while (j <= right) temp[k++] = data[j++];
            for (i = 0; i < k; i++) data[left + i] = temp[i];
            mtx.unlock();
            if (window) {
                QMetaObject::invokeMethod(window, [window] {
                    window->drawData();
                }, Qt::QueuedConnection);
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(window->getDelay()));
        }
    }
}
