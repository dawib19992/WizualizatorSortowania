#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "AlgorytmSortowania.h"
#include <random>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    int rozmiarDanych = 500;
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
    delete ui;
}

void MainWindow::drawData(){
    scena->clear();
    double width = ui->gv_wizualizator->width() / (double)data.size();
    int i = 0;
    for(auto singleData : std::as_const(data)){
        scena->addRect(QRect((width * i), ui->gv_wizualizator->height() - singleData, (width / 2), singleData), QPen(Qt::black), QBrush(Qt::red));
        i++;
    }

}

void MainWindow::setData(int noData){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(10, 200);
    data.resize(noData);
    for (int& val : data) val = dis(gen);
}

void MainWindow::on_hs_rozmiarDanych_sliderMoved(int position)
{
    ui->lbl_rozmiarDanych->setText(QString::number(position));
    setData(position);
    drawData();
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


QString MainWindow::toString(Type type){
    switch(type){
    case Type::MergeSort:
        return "MergeSort";
        break;
    default:
        return "Not a algoritm type";
        break;
    }
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

void MainWindow::on_hs_szybkosc_sliderMoved(int position)
{
    ui->lbl_szybkosc->setText(QString::number(position));
}

