#ifndef EVENTDETECTIONDIALOG_H
#define EVENTDETECTIONDIALOG_H

#include <QDialog>

namespace Ui {
class EventDetectionDialog;
}

class EventDetectionDialog : public QDialog
{
    Q_OBJECT

public:
    QWidget *mainSub;
    QVector<double> histBins;
    QVector<double> binVals;
    explicit EventDetectionDialog(QWidget *parent = nullptr);
    ~EventDetectionDialog();

private slots:
    void on_checkBox_stateChanged(int arg1);

    void on_lineEdit_thresh_textChanged(const QString &arg1);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::EventDetectionDialog *ui;
};

#endif // EVENTDETECTIONDIALOG_H
