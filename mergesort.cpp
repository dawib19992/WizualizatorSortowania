#include "mergesort.h"
#include <thread>
#include <chrono>

void MergeSort::merge(std::vector<int>& data, int left, int mid, int right, std::mutex& mtx) {
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

void MergeSort::mergeSort(std::vector<int>& data, int left, int right, std::mutex& mtx, bool& paused, bool& reset) {
   if (left >= right || reset) {
        return;
    }

    while (paused && !reset) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    if (reset) {
        return;
    }

    int mid = left + (right - left) / 2;

    if (right - left > 100) {
        std::thread leftThread(&MergeSort::mergeSort, this, std::ref(data), left, mid, std::ref(mtx), std::ref(paused), std::ref(reset));
        mergeSort(data, mid + 1, right, mtx, paused, reset);
        leftThread.join(); // Czekamy na zakończenie wątku
    } else {
        mergeSort(data, left, mid, mtx, paused, reset);
        mergeSort(data, mid + 1, right, mtx, paused, reset);
    }

    while (paused && !reset) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    if (reset) {
        return;
    }

    mtx.lock();
    merge(data, left, mid, right, mtx);
    mtx.unlock();

    std::this_thread::sleep_for(std::chrono::milliseconds(50));
}

void MergeSort::sort(std::vector<int>& data, std::mutex& mtx, bool& paused, bool& reset) {
    operations = 0;
    mergeSort(data, 0, data.size() - 1, mtx, paused, reset);
}
