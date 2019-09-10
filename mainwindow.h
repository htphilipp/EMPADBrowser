#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include "histogramdialog.h"
#include "plotpixels.h"
#include "histogramareadialog.h"
#include "moviedialog.h"
#include "calcdebounceoffsets.h"
#include "sumframesdialog.h"
#include<opencv2/opencv.hpp>
#include<opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include "empad2lib.h"
#include <boost/filesystem.hpp>
#include <vector>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QString rawfilename;
    QString rawBGfilename;
    Empad2 *rawData;
    Empad2 *rawBGData;
    void updateDisplay();
    cv::Mat adjMap;
    cv::Mat combined;
    cv::Mat scaledDig;
    cv::Mat scaledAna;
    cv::Mat scaledGain;
    cv::Mat dark;
    cv::Mat results;

    std::vector<double> offsets;

    double anaDisplayScale,digDisplayScale,comDisplayScale;
    static void onMouseDig(int, int, int, int, void*);
    static void onMouseAna(int, int, int, int, void*);
    static void onMouseComb(int, int, int, int, void*);
    static void onMouseResults(int, int, int, int, void*);
    static void onMouseGain(int, int, int, int, void*);
    double getScaleFactor();

private slots:
    void on_pushButton_clicked();

    void on_lineEdit_editingFinished();

    void on_pushButton_2_clicked();

    void on_lineEdit_2_editingFinished();

    void on_lineEdit_textChanged(const QString &arg1);

    void on_lineEdit_2_textChanged(const QString &arg1);

    void on_pushButton_3_clicked();

    void on_frameSlider_valueChanged(int value);

    void on_lineEdit_4_textEdited(const QString &arg1);

    void on_lineEdit_5_textEdited(const QString &arg1);

    void on_lineEdit_6_textEdited(const QString &arg1);

    void on_pushButton_4_clicked();

    void on_checkBox_stateChanged(int arg1);

    void on_actionHistogram_Pixel_triggered();

    void on_actionPlot_Pixel_Values_triggered();

    void on_actionHistogram_Area_triggered();

    void on_actionSave_Video_triggered();

    void on_actionCalc_Debounce_Offsets_triggered();

    void on_actionAdd_Frames_triggered();

    void on_lineEdit_resultsScale_textEdited(const QString &arg1);

private:
    Ui::MainWindow *ui;


    HistogramDialog *histui;
    HistogramAreaDialog *histAreaui;
    PlotPixels *pltPix;
    movieDialog *movDialog;
    CalcDebounceOffsets *calcOffsets;
    SumFramesDialog *sumFrames;

    friend class HistogramDialog;
    friend class plotpixels;
    friend class HistogramAreaDialog;
    friend class movieDialog;
    friend class CalcDebounceOffsets;
    friend class SumFramesDialog;
};

#endif // MAINWINDOW_H
