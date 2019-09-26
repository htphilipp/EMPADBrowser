#include "calibrationdialog.h"
#include "ui_calibrationdialog.h"
#include "mainwindow.h"
#include <boost/filesystem.hpp>
#include <boost/iostreams/device/mapped_file.hpp>
#include <boost/iostreams/stream.hpp>


CalibrationDialog::CalibrationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CalibrationDialog)
{
    ui->setupUi(this);
    mainSub = parent;
}

CalibrationDialog::~CalibrationDialog()
{
    delete ui;
}

void CalibrationDialog::on_pushButton_constDark_clicked()
{
    MainWindow *mainSubLocal = reinterpret_cast<MainWindow*>(mainSub);
    mainSubLocal->darks[0]    =   cv::Mat(128,128,CV_64F,cv::Scalar(ui->lineEdit_darks0->text().toDouble()));
    mainSubLocal->darks[1]    =   cv::Mat(128,128,CV_64F,cv::Scalar(ui->lineEdit_darks1->text().toDouble()));
}


void CalibrationDialog::on_pushButton_constRatio_clicked()
{
    MainWindow *mainSubLocal = reinterpret_cast<MainWindow*>(mainSub);
    mainSubLocal->sRatio[0]   =   cv::Mat(128,128,CV_64F,cv::Scalar(ui->lineEdit_ratio0->text().toDouble()));
    mainSubLocal->sRatio[1]   =   cv::Mat(128,128,CV_64F,cv::Scalar(ui->lineEdit_ratio1->text().toDouble()));
}

void CalibrationDialog::on_pushButton_constOffset_clicked()
{
    MainWindow *mainSubLocal = reinterpret_cast<MainWindow*>(mainSub);
    mainSubLocal->lgOffset[0] =   cv::Mat(128,128,CV_64F,cv::Scalar(ui->lineEdit_offset0->text().toDouble()));
    mainSubLocal->lgOffset[1] =   cv::Mat(128,128,CV_64F,cv::Scalar(ui->lineEdit_offset1->text().toDouble()));
}

void CalibrationDialog::on_pushButton_constDump_clicked()
{
    MainWindow *mainSubLocal = reinterpret_cast<MainWindow*>(mainSub);
    mainSubLocal->dADUdN[0]   =   cv::Mat(128,128,CV_64F,cv::Scalar(ui->lineEdit_dADUdN0->text().toDouble()));
    mainSubLocal->dADUdN[1]   =   cv::Mat(128,128,CV_64F,cv::Scalar(ui->lineEdit_dADUdN1->text().toDouble()));
}



void CalibrationDialog::on_pushButton_writeCalibFile_clicked()
{
    MainWindow *mainSubLocal = reinterpret_cast<MainWindow*>(mainSub);
    QString filename;
    filename        = QFileDialog::getSaveFileName(this, tr("Save calibration to:"), "f:/", tr("*.calib"));
    boost::iostreams::mapped_file_params params;
    params.path     = filename.toStdString();
    params.new_file_size =  sizeof(uint64_t)*128*128*8;
    // calibration file 8 64-bit float frames
    //in this order:
    // even-dark,odd-dark,even-ratio,odd-ratio,even offset, odd offset, even charge dump, odd charge dump
    params.flags         = boost::iostreams::mapped_file::mapmode::readwrite;
    cv::Mat *output;
    int i = 0;

    output = nullptr;
    boost::iostreams::mapped_file_sink out(params);

    output = new cv::Mat(int(128),int(128),CV_64F,reinterpret_cast<uchar *>(const_cast<char *>(out.data()))+(i*128*128*sizeof(uint64_t)));
    (mainSubLocal->darks[0]).copyTo(*output);
    delete output;
    i++;
    output = new cv::Mat(int(128),int(128),CV_64F,reinterpret_cast<uchar *>(const_cast<char *>(out.data()))+(i*128*128*sizeof(uint64_t)));
    (mainSubLocal->darks[1]).copyTo(*output);
    delete output;
    i++;
    output = new cv::Mat(int(128),int(128),CV_64F,reinterpret_cast<uchar *>(const_cast<char *>(out.data()))+(i*128*128*sizeof(uint64_t)));
    (mainSubLocal->sRatio[0]).copyTo(*output);
    delete output;
    i++;
    output = new cv::Mat(int(128),int(128),CV_64F,reinterpret_cast<uchar *>(const_cast<char *>(out.data()))+(i*128*128*sizeof(uint64_t)));
    (mainSubLocal->sRatio[1]).copyTo(*output);
    delete output;
    i++;
    output = new cv::Mat(int(128),int(128),CV_64F,reinterpret_cast<uchar *>(const_cast<char *>(out.data()))+(i*128*128*sizeof(uint64_t)));
    (mainSubLocal->lgOffset[0]).copyTo(*output);
    delete output;
    i++;
    output = new cv::Mat(int(128),int(128),CV_64F,reinterpret_cast<uchar *>(const_cast<char *>(out.data()))+(i*128*128*sizeof(uint64_t)));
    (mainSubLocal->lgOffset[1]).copyTo(*output);
    delete output;
    i++;
    output = new cv::Mat(int(128),int(128),CV_64F,reinterpret_cast<uchar *>(const_cast<char *>(out.data()))+(i*128*128*sizeof(uint64_t)));
    (mainSubLocal->dADUdN[0]).copyTo(*output);
    delete output;
    i++;
    output = new cv::Mat(int(128),int(128),CV_64F,reinterpret_cast<uchar *>(const_cast<char *>(out.data()))+(i*128*128*sizeof(uint64_t)));
    (mainSubLocal->dADUdN[1]).copyTo(*output);
    delete output;

    if(out.is_open())
    {
        out.close();
    }
}


void CalibrationDialog::on_pushButton_readCalibFile_clicked()
{
    MainWindow *mainSubLocal = reinterpret_cast<MainWindow*>(mainSub);
    QString filename;
    filename        = QFileDialog::getOpenFileName(this, tr("Load calibration from:"), "f:/", tr("*.calib"));
    uint sizeOfFile;
    // calibration file 8 64-bit float frames
    //in this order:
    // even-dark,odd-dark,even-ratio,odd-ratio,even offset, odd offset, even charge dump, odd charge dump

    cv::Mat *input;
    int i = 0;

    input = nullptr;
    boost::iostreams::mapped_file_source infile;
    infile.open(filename.toStdString());
    if(infile.size() == sizeof(uint64_t)*128*128*8)
    {
        input = new cv::Mat(int(128),int(128),CV_64F,reinterpret_cast<uchar *>(const_cast<char *>(infile.data()))+(i*128*128*sizeof(uint64_t)));
        (*input).copyTo(mainSubLocal->darks[0]);
        delete input;
        i++;
        input = new cv::Mat(int(128),int(128),CV_64F,reinterpret_cast<uchar *>(const_cast<char *>(infile.data()))+(i*128*128*sizeof(uint64_t)));
        (*input).copyTo(mainSubLocal->darks[1]);
        delete input;
        i++;
        input = new cv::Mat(int(128),int(128),CV_64F,reinterpret_cast<uchar *>(const_cast<char *>(infile.data()))+(i*128*128*sizeof(uint64_t)));
        (*input).copyTo(mainSubLocal->sRatio[0]);
        delete input;
        i++;
        input = new cv::Mat(int(128),int(128),CV_64F,reinterpret_cast<uchar *>(const_cast<char *>(infile.data()))+(i*128*128*sizeof(uint64_t)));
        (*input).copyTo(mainSubLocal->sRatio[0]);
        delete input;
        i++;

        input = new cv::Mat(int(128),int(128),CV_64F,reinterpret_cast<uchar *>(const_cast<char *>(infile.data()))+(i*128*128*sizeof(uint64_t)));
        (*input).copyTo(mainSubLocal->lgOffset[0]);
        delete input;
        i++;
        input = new cv::Mat(int(128),int(128),CV_64F,reinterpret_cast<uchar *>(const_cast<char *>(infile.data()))+(i*128*128*sizeof(uint64_t)));
        (*input).copyTo(mainSubLocal->lgOffset[1]);
        delete input;
        i++;

        input = new cv::Mat(int(128),int(128),CV_64F,reinterpret_cast<uchar *>(const_cast<char *>(infile.data()))+(i*128*128*sizeof(uint64_t)));
        (*input).copyTo(mainSubLocal->dADUdN[0]);
        delete input;
        i++;
        input = new cv::Mat(int(128),int(128),CV_64F,reinterpret_cast<uchar *>(const_cast<char *>(infile.data()))+(i*128*128*sizeof(uint64_t)));
        (*input).copyTo(mainSubLocal->dADUdN[1]);
        delete input;
    }
    else
    {
        ui->label_message->setText(QString("Calibration file not the correct format"));
    }
    if(infile.is_open())
    {
        infile.close();
    }
}


