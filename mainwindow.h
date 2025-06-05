#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "AlgorytmSortowania.h"
#include <QMainWindow>
#include <qgraphicsscene.h>

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
    void drawData();
    void setData(int noData);
    void setAlgorithms();
    void setScene();
    void setSliders(int rd, int s);
    QString toString(Type type);

private slots:
    void on_hs_rozmiarDanych_sliderMoved(int position);

    void on_hs_szybkosc_sliderMoved(int position);

private:
    Ui::MainWindow *ui;
    QGraphicsScene* scena;
    QVector<int> data;
    QVector<Type> types;
};
#endif // MAINWINDOW_H
