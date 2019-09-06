#ifndef MOVIEDIALOG_H
#define MOVIEDIALOG_H

#include <QDialog>
#include<opencv2/opencv.hpp>
#include<opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

namespace Ui {
class movieDialog;
}

class movieDialog : public QDialog
{
    Q_OBJECT

public:
    explicit movieDialog(QWidget *parent = nullptr);
    QWidget *mainSub;

    ~movieDialog();

private slots:
    void on_pushButton_clicked();

private:
    Ui::movieDialog *ui;
};

#endif // MOVIEDIALOG_H
