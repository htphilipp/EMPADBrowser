#ifndef PLOTPIXELS_H
#define PLOTPIXELS_H

//#include "mainwindow.h"
#include <QDialog>
#include "qcustomplot.h"
#include <QVector>

namespace Ui {
class PlotPixels;
}

class PlotPixels : public QDialog
{
    Q_OBJECT

public:
    explicit PlotPixels(QWidget *parent = nullptr);
    QWidget *mainSub;
    QVector<double> evenframenum;
    QVector<double> oddframenum;
    QVector<double> evenFrames;
    QVector<double> oddFrames;

    int xpix,ypix;

    ~PlotPixels();

private slots:
    void on_pushButton_collectVals_clicked();

    void on_checkBox_showEven_stateChanged(int arg1);

    void on_checkBox_showOdd_stateChanged(int arg1);

    void on_pushButton_expData_clicked();

    void on_checkBox_plotDots_stateChanged(int arg1);

    void on_lineEdit_pointsize_textChanged(const QString &arg1);

private:
    Ui::PlotPixels *ui;
};

#endif // PLOTPIXELS_H
