#include "calcdebounceoffsets.h"
#include "ui_calcdebounceoffsets.h"
#include "mainwindow.h"

CalcDebounceOffsets::CalcDebounceOffsets(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CalcDebounceOffsets)
{
    ui->setupUi(this);
    mainSub = parent;
}

CalcDebounceOffsets::~CalcDebounceOffsets()
{
    delete ui;
}

void CalcDebounceOffsets::on_pushButton_calc_clicked()
{
    MainWindow *mainSubLocal = reinterpret_cast<MainWindow*>(mainSub);
    uint numframes = mainSubLocal->rawData->getNumOfFrames();
    mainSubLocal->offsets.clear();
    uint frameHold = mainSubLocal->rawData->getFrameNum();
    tempMatImg = cv::Mat(128,128,CV_64F,cv::Scalar(0));
    tempMatImgVals = cv::Mat(128,128,CV_64F,cv::Scalar(0));
    tempMatImgCount = cv::Mat(128,128,CV_64F,cv::Scalar(0));

    double psum;
    double pcount;

    uint xmin,xmax,ymin,ymax;

    if(ui->lineEdit_x1->text().toUInt()<ui->lineEdit_x2->text().toUInt())
    {
        xmin = ui->lineEdit_x1->text().toUInt();
        xmax = ui->lineEdit_x2->text().toUInt();
    }
    else
    {
        xmax = ui->lineEdit_x1->text().toUInt();
        xmin = ui->lineEdit_x2->text().toUInt();
    }

    if(ui->lineEdit_y1->text().toUInt()<ui->lineEdit_y2->text().toUInt())
    {
        ymin = ui->lineEdit_y1->text().toUInt();
        ymax = ui->lineEdit_y2->text().toUInt();
    }
    else
    {
        ymax = ui->lineEdit_y1->text().toUInt();
        ymin = ui->lineEdit_y2->text().toUInt();
    }


    for(uint i = 0; i< numframes; i++)
    {
        mainSubLocal->rawData->goToFrame(i);
        mainSubLocal->rawData->imgAnalog->convertTo(tempMatImg,CV_64F);
        tempMatImg = tempMatImg - (mainSubLocal->dark);
        cv::threshold(tempMatImg,tempMatImgVals,ui->lineEdit_threshold->text().toDouble(),0,4);
        cv::threshold(tempMatImg,tempMatImgCount,ui->lineEdit_threshold->text().toDouble(),1,1);
        psum = 0;
        pcount = 0;
        for(uint xi = xmin; xi<=xmax; xi++)
        {
            for(uint yi = ymin; yi<=ymax;yi++)
            {
                    psum += tempMatImgVals.at<double>(int(yi),int(xi));
                    pcount += tempMatImgCount.at<double>(int(yi),int(xi));
            }

        }
        mainSubLocal->offsets.push_back(psum/pcount);
    }
    mainSubLocal->rawData->goToFrame(frameHold);
}

void CalcDebounceOffsets::on_pushButton_clicked()
{
    MainWindow *mainSubLocal = reinterpret_cast<MainWindow*>(mainSub);
    QString filenameToSave= QFileDialog::getSaveFileName(this,tr("File to save data to"), "c:/", tr("csv Files (*.csv)"));

    QFile output(filenameToSave);
    output.open(QFile::WriteOnly|QFile::Truncate|QIODevice::Text);
    QTextStream stream(&output);
    stream<<"offsets"<<"\n";
    stream<<"even,   odd  \n";
    for(auto i = 0; i<int(mainSubLocal->offsets.size());i++)
    {
        stream<<double(mainSubLocal->offsets[uint(i)]);
        if(i%2)
            stream<<"\n";
        else
            stream<<", ";
    }
    output.close();
}
