#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "AlgorytmSortowania.h"
#include "mergesort.h"
#include "bubblesort.h"
#include "quicksort.h"
#include <random>
#include <QMessageBox>

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
    std::uniform_real_distribution<> dis(10, 500);
    data.resize(ui->hs_rozmiarDanych->value());
    for (float& val : data) val = dis(gen);
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
        algorithm->sort(data, mtx, paused, reset, this);

        if (!reset) {
            QMetaObject::invokeMethod(this, [this] {
                drawData();
            }, Qt::QueuedConnection);

            bool sortedCorrectly = this->checkIfSorted();

            QMetaObject::invokeMethod(this, [this, sortedCorrectly] {
                if (sortedCorrectly) {
                    QMessageBox::information(this, "Wynik", "Dane zostały poprawnie posortowane.");
                } else {
                    QMessageBox::warning(this, "Błąd", "Dane nie są poprawnie posortowane.");
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
        algorithm = new class MergeSort();
        break;
    case Type::BubbleSort:
        algorithm = new class BubbleSort();
        break;
    case Type::QuickSort:
        algorithm = new class QuickSort();
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
}

void MainWindow::on_pb_stop_clicked()
{
    resetSorting();
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
    switch(type){
    case Type::MergeSort:
        return "Merge Sort";
        break;
    case Type::BubbleSort:
        return "Bubble Sort";
        break;
    case Type::QuickSort:
        return "Quick Sort";
    default:
        return "Not a algoritm type";
        break;
    }
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
                drawData(i, i + 1); // Pokaż porównanie
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
        drawData(); // końcowe odświeżenie
    }, Qt::QueuedConnection);

    return true;
}


