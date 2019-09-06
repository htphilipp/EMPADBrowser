#ifndef HISTOGRAMDIALOG_H
#define HISTOGRAMDIALOG_H

#include <QDialog>
#include <QtCharts>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
//#include <QMainWindow>
//#include "mainwindow.h"
#include <vector>
#include "qcustomplot.h"
#include <QVector>

namespace Ui {
class HistogramDialog;
}

class HistogramDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HistogramDialog(QWidget *parent = nullptr);

    ~HistogramDialog();
    QWidget *mainSub;
//    std::vector<double> pixelDat;
//    std::vector<double> histBins;
//    std::vector<int> binVals;
    QVector<double> pixelDat;
    QVector<double> histBins;
    QVector<double> binVals;

    QVector<QVector <double>> multiPixDat;
    QVector<QVector <double>> pixCoords;

private slots:
    void on_pushButton_collectVals_clicked();

    void on_pushButton_MakeBinPlot_clicked();

    void on_pushButton_expData_clicked();

    void on_pushButton_clicked();

private:
    Ui::HistogramDialog *ui;
};

#endif // HISTOGRAMDIALOG_H
