#include "bubblesort.h"
#include <qnamespace.h>
#include <qobjectdefs.h>
#include <thread>

void BubbleSort::sort(std::vector<float>& data, std::mutex& mtx, bool& paused, bool& reset, MainWindow* window){
    operations = 0;
    int n = data.size();
    if (n <= 1) return;

    for(int i = 0; i < n; i++){
        for(int j = 0; j < n - i -1; j++){
            while(paused &&  reset){
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
            if(reset){
                return;
            }
            mtx.lock();
            if (window) {
                QMetaObject::invokeMethod(window, [window, i, j] {
                    window->drawData(i,j);
                }, Qt::QueuedConnection);
            }
            if(data[j] > data[j+1]){
                std::swap(data[j], data[j+1]);
                operations++;
            }
            mtx.unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds(window->getDelay()));
        }
    }
}
