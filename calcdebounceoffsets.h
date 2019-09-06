#ifndef CALCDEBOUNCEOFFSETS_H
#define CALCDEBOUNCEOFFSETS_H

#include <QDialog>
#include<opencv2/opencv.hpp>
#include<opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

namespace Ui {
class CalcDebounceOffsets;
}

class CalcDebounceOffsets : public QDialog
{
    Q_OBJECT

public:
    QWidget *mainSub;
    cv::Mat tempMatImg;
    cv::Mat tempMatImgVals;
    cv::Mat tempMatImgCount;
    long normPixels;

    explicit CalcDebounceOffsets(QWidget *parent = nullptr);
    ~CalcDebounceOffsets();

private slots:
    void on_pushButton_calc_clicked();

    void on_pushButton_clicked();

private:
    Ui::CalcDebounceOffsets *ui;
};

#endif // CALCDEBOUNCEOFFSETS_H
