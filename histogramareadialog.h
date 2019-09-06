#ifndef HISTOGRAMAREADIALOG_H
#define HISTOGRAMAREADIALOG_H

#include <QDialog>

namespace Ui {
class HistogramAreaDialog;
}

class HistogramAreaDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HistogramAreaDialog(QWidget *parent = nullptr);
    ~HistogramAreaDialog();
    QWidget *mainSub;
    QVector<double> pixelDat;
    QVector<double> histBins;
    QVector<double> binVals;

private slots:
    void on_pushButton_collectVals_clicked();

    void on_pushButton_MakeBinPlot_clicked();

    void on_checkBox_stateChanged(int arg1);

    void on_pushButton_expData_clicked();

private:
    Ui::HistogramAreaDialog *ui;
};

#endif // HISTOGRAMAREADIALOG_H
