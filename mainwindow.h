#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "AlgorytmSortowania.h"
#include "ui_mainwindow.h"
#include <QMainWindow>
#include <qgraphicsscene.h>
#include <thread>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void drawData(int h1 = -1, int h2 = -1);
    void setData(int noData);
    void setAlgorithms();
    void setScene();
    void setSliders(int rd, int s);
    QString toString(Type type);
    void startSorting();
    void pauseSorting();
    void resetSorting();
    void changeAlgorithm(int index);
    int getDelay() {return ui->hs_szybkosc->value();}

private slots:
    void on_hs_rozmiarDanych_sliderMoved(int position);

    void on_hs_szybkosc_sliderMoved(int position);

    void on_cb_typ_currentIndexChanged(int index);

    void on_pb_start_clicked();

    void on_pb_stop_clicked();

    void on_pb_pauza_clicked();

private:
    Ui::MainWindow *ui;
    QGraphicsScene* scena;
    std::vector<float> data;
    QVector<Type> types;
    SortingAlgorithm* algorithm;
    std::thread sortingThread;
    std::mutex mtx;
    bool paused = false;
    bool reset = false;
};
#endif // MAINWINDOW_H
