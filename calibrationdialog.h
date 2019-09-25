#ifndef CALIBRATIONDIALOG_H
#define CALIBRATIONDIALOG_H

#include <QDialog>

namespace Ui {
class CalibrationDialog;
}

class CalibrationDialog : public QDialog
{
    Q_OBJECT

public:
    QWidget *mainSub;
    explicit CalibrationDialog(QWidget *parent = nullptr);
    ~CalibrationDialog();

private slots:
    void on_pushButton_constDark_clicked();

    void on_pushButton_constRatio_clicked();

    void on_pushButton_constOffset_clicked();

    void on_pushButton_constDump_clicked();

    void on_pushButton_writeCalibFile_clicked();

    void on_pushButton_readCalibFile_clicked();

private:
    Ui::CalibrationDialog *ui;
};

#endif // CALIBRATIONDIALOG_H
