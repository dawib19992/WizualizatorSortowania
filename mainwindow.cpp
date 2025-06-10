#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "AlgorytmSortowania.h"
#include "mergesort.h"
#include "bubblesort.h"
#include "quicksort.h"
#include "heapsort.h"
#include "radixsort.h"
#include "bogosort.h"
#include <random>
#include <QMessageBox>
#include <QElapsedTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    int rozmiarDanych = 250;
    int maksSzybkosc = 600; //w sekundach
    setSliders(rozmiarDanych, maksSzybkosc);
    setScene();
    ui->gv_wizualizator->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->gv_wizualizator->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setData(ui->hs_rozmiarDanych->value());
    drawData();
    setAlgorithms();
}

MainWindow::~MainWindow()
{
    reset = true;
    if (sortingThread.joinable()) sortingThread.join();
    delete ui;
}

void MainWindow::drawData(int h1, int h2){
    scena->clear();
    double width = ui->gv_wizualizator->width() / (double)data.size();
    int i = 0;
    for(auto singleData : std::as_const(data)){
        QBrush brush = (i == h1 || i == h2) ? QBrush(Qt::red) : QBrush(Qt::blue);
        scena->addRect(QRect((width * i), ui->gv_wizualizator->height() - singleData, (width / 2), singleData), QPen(Qt::black), brush);
        i++;
    }
}

void MainWindow::setData(int noData){

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(10, 500);
    data.resize(ui->hs_rozmiarDanych->value());
    for (int& val : data) val = dis(gen);
}

void MainWindow::on_hs_rozmiarDanych_sliderMoved(int position)
{
    ui->lbl_rozmiarDanych->setText(QString::number(position));
    setData(position);
    drawData();
    resetSorting();
}

void MainWindow::setAlgorithms(){
    for(int i = 0; i < Type::Count; i++){
        Type type = static_cast<Type>(i);
        types.append(type);
        ui->cb_typ->addItem(toString(type));
    }
}

void MainWindow::setScene(){
    int x = ui->gv_wizualizator->x();
    int y = ui->gv_wizualizator->y();
    int width = ui->gv_wizualizator->width() - 25;
    int height = ui->gv_wizualizator->height();
    scena = new QGraphicsScene(x, y, width, height);
    ui->gv_wizualizator->setScene(scena);
}


void MainWindow::setSliders(int rd, int s){
    ui->hs_rozmiarDanych->setMaximum(rd);
    ui->hs_rozmiarDanych->setMinimum(10);
    ui->hs_rozmiarDanych->setValue(rd/2.0);
    ui->lbl_rozmiarDanych->setText(QString::number(ui->hs_rozmiarDanych->value()));
    ui->hs_szybkosc->setMaximum(s);
    ui->hs_szybkosc->setMinimum(10);
    ui->hs_szybkosc->setValue(s/2.0);
    ui->lbl_szybkosc->setText(QString::number(ui->hs_szybkosc->value()));
}

void MainWindow::startSorting(){
    if(sortingThread.joinable()) return;
    paused = false;
    reset = false;

    sortingThread = std::thread([this]{
        QElapsedTimer timer;
        timer.start();

        algorithm->sort(data, mtx, paused, reset, this);

        double elapsed = timer.nsecsElapsed() / 1000000000.0;

        if (!reset) {            
            QMetaObject::invokeMethod(this, [this] {
                drawData();
            }, Qt::QueuedConnection);

            bool sortedCorrectly = this->checkIfSorted();

            QMetaObject::invokeMethod(this, [this, sortedCorrectly, elapsed] {
                QString message;
                message = QString("%1\nCzas: %2 s\nOperacje: %3")
                              .arg(sortedCorrectly ? "Dane zostały poprawnie posortowane." : "Dane NIE są poprawnie posortowane.")
                              .arg(QString::number(elapsed, 'f', 3))
                              .arg(algorithm->getOperations());
                if (sortedCorrectly) {
                    QMessageBox::information(this, "Wynik", message);
                } else {
                    QMessageBox::warning(this, "Błąd", message);
                }
            }, Qt::QueuedConnection);
        }
    });
}

void MainWindow::pauseSorting(){
    paused = !paused;
    ui->pb_pauza->setText(paused ? "Wznów" : "Pauza");
}
void MainWindow::resetSorting(){
    reset = true;
    if (sortingThread.joinable()) sortingThread.join();
    setData(ui->hs_rozmiarDanych->value());
    drawData();
    reset = false;
}

void MainWindow::changeAlgorithm(int index){
    Type type = static_cast<Type>(index);
    switch(type){
    case Type::MergeSort:
        algorithm = std::make_unique<class MergeSort>();
        break;
    case Type::BubbleSort:
        algorithm = std::make_unique<class BubbleSort>();
        break;
    case Type::QuickSort:
        algorithm = std::make_unique<class QuickSort>();
        break;
    case Type::RadixSort:
        algorithm = std::make_unique<class RadixSort>();
        break;
    case Type::HeapSort:
        algorithm = std::make_unique<class HeapSort>();
        break;
    case Type::BogoSort:
        algorithm = std::make_unique<class BogoSort>();
        break;

    default:
        break;
    }
    resetSorting();
}

void MainWindow::on_cb_typ_currentIndexChanged(int index)
{
    changeAlgorithm(index);
}

void MainWindow::on_pb_start_clicked()
{
    startSorting();
    ui->pb_start->setEnabled(false);
    ui->hs_rozmiarDanych->setEnabled(false);
}

void MainWindow::on_pb_stop_clicked()
{
    resetSorting();
    ui->pb_start->setEnabled(true);
    ui->hs_rozmiarDanych->setEnabled(true);
}

void MainWindow::on_pb_pauza_clicked()
{
    pauseSorting();
}


void MainWindow::on_hs_szybkosc_sliderMoved(int position)
{
    ui->lbl_szybkosc->setText(QString::number(position));
}


QString MainWindow::toString(Type type){
    QString toReturn;
    switch(type){
    case Type::MergeSort:
        toReturn = "Merge Sort";
        break;
    case Type::BubbleSort:
        toReturn = "Bubble Sort";
        break;
    case Type::QuickSort:
        toReturn = "Quick Sort";
        break;
    case Type::RadixSort:
        toReturn = "Radix Sort";
        break;
    case Type::HeapSort:
        toReturn = "Heap Sort";
        break;
    case Type::BogoSort:
        toReturn = "Bogo Sort";
        break;
    default:
        toReturn = "Not a algoritm type";
        break;
    }
    return toReturn;
}

bool MainWindow::checkIfSorted()
{
    int n = data.size();
    if (n <= 1) return true;

    for (int i = 0; i < n - 1; ++i)
    {
        while (paused && !reset)
            std::this_thread::sleep_for(std::chrono::milliseconds(100));

        if (reset) return false;

        {
            std::lock_guard<std::mutex> lock(mtx);

            QMetaObject::invokeMethod(this, [this, i] {
                drawData(i, i + 1);
            }, Qt::QueuedConnection);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(getDelay()));

        {
            std::lock_guard<std::mutex> lock(mtx);
            if (data[i] > data[i + 1])
                return false;
        }
    }

    QMetaObject::invokeMethod(this, [this] {
        drawData();
    }, Qt::QueuedConnection);

    return true;
}


