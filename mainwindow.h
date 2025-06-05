#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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

private slots:
    void on_hs_rozmiarDanych_sliderMoved(int position);

private:
    Ui::MainWindow *ui;
    QGraphicsScene* scena;
    QVector<int> data;
};
#endif // MAINWINDOW_H
