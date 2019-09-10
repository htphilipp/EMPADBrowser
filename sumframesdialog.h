#ifndef SUMFRAMESDIALOG_H
#define SUMFRAMESDIALOG_H

#include <QDialog>
#include<opencv2/opencv.hpp>
#include<opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

namespace Ui {
class SumFramesDialog;
}

class SumFramesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SumFramesDialog(QWidget *parent = nullptr);
    ~SumFramesDialog();
    QWidget *mainSub;
    uint startFrame;
    uint endFrame;
    uint step;
    cv::Mat tempMat;

private slots:
    void on_pushButton_collectVals_clicked();

private:
    Ui::SumFramesDialog *ui;
};

#endif // SUMFRAMESDIALOG_H
