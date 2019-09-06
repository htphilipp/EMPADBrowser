#include "moviedialog.h"
#include "ui_moviedialog.h"
#include "mainwindow.h"
#include <opencv2/videoio.hpp>
//#include <opencv2/videoio/cap_ios.h>

movieDialog::movieDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::movieDialog)
{
    ui->setupUi(this);
    mainSub = parent;
}

movieDialog::~movieDialog()
{
    delete ui;
}

void movieDialog::on_pushButton_clicked()
{
    MainWindow *mainSubLocal = reinterpret_cast<MainWindow*>(mainSub);
    cv::Mat tempMat = cv::Mat(int(mainSubLocal->rawData->getxdim()),3*int(mainSubLocal->rawData->getydim()),CV_64F,cv::Scalar(0));
    cv::Mat tempMat8bit = cv::Mat(int(mainSubLocal->rawData->getxdim()),3*int(mainSubLocal->rawData->getydim()),CV_8U,cv::Scalar(0));
    //cv::Mat tempMat8bitc3 = cv::Mat(int(mainSubLocal->rawData->getxdim()),3*int(mainSubLocal->rawData->getydim()),CV_8UC3,cv::Scalar(0));
    cv::Mat tempSized;


    int frameBegin = ui->lineEdit_begin->text().toInt();
    int frameEnd = ui->lineEdit_end->text().toInt();
    int tempnum;
    cv::VideoWriter vidWrite;
    QString filename;
    int xdim = int(mainSubLocal->rawData->getxdim());
    int ydim = int(mainSubLocal->rawData->getydim());
    cv::namedWindow("output video");

    uint frameNumHold = mainSubLocal->rawData->getFrameNum();

    if(frameBegin<0)
        frameBegin = 0;

    if(frameEnd<0)
        frameEnd = 0;

    if (frameBegin > (mainSubLocal->rawData->getNumOfFrames()))
        frameBegin = (mainSubLocal->rawData->getNumOfFrames())-1;

    if (frameBegin > (mainSubLocal->rawData->getNumOfFrames()))
        frameBegin = (mainSubLocal->rawData->getNumOfFrames())-1;

    if(frameBegin>frameEnd)
    {
        tempnum = frameBegin;
        frameBegin = frameEnd;
        frameEnd = tempnum;
    }

    filename = QFileDialog::getSaveFileName(this,
                 tr("Save movie to"), "f:/", tr("*.avi"));

    //vidWrite.open(filename.toStdString(),0x00000021,10, cv::Size(3*int(mainSubLocal->rawData->getxdim()),int(mainSubLocal->rawData->getydim())));
    //vidWrite.open(filename.toStdString(),cv::VideoWriter::fourcc('M','J','P','G'),20, cv::Size(128,3*128),true); //explicit dimensions for testing
    //vidWrite.open(filename.toStdString(),-1,20, cv::Size(128,3*128),true); //explicit dimensions for testing
    vidWrite.open(filename.toStdString(),cv::VideoWriter::fourcc('M','J','P','G'),20, cv::Size(7*3*int(mainSubLocal->rawData->getxdim()),7*int(mainSubLocal->rawData->getydim())),false); //explicit dimensions for testing
    //vidWrite.open(filename.toStdString(),cv::VideoWriter::fourcc('M','P','G','1'),20, cv::Size(4*3*int(mainSubLocal->rawData->getxdim()),4*int(mainSubLocal->rawData->getydim())),false); //explicit dimensions for testing


    if (!vidWrite.isOpened())
    {

        ui->label_3->setText(QString("problem with output file."));
    }
    else
    {
        for(auto i = frameBegin; i<=frameEnd; i=i+ui->lineEdit_step->text().toInt())
        {
            mainSubLocal->rawData->goToFrame(i);
            mainSubLocal->updateDisplay();
           // tempMat(cv::Rect(0,0,xdim,ydim)) = mainSubLocal->scaledAna;
           //tempMat(cv::Rect(xdim,0,xdim,ydim)) = mainSubLocal->scaledDig;
           // tempMat(cv::Rect(2*xdim,0,xdim,ydim)) = mainSubLocal->combined;

           cv::convertScaleAbs(mainSubLocal->scaledAna, tempMat8bit(cv::Rect(0,0,xdim,ydim)), mainSubLocal->anaDisplayScale);
           cv::convertScaleAbs(mainSubLocal->scaledDig, tempMat8bit(cv::Rect(xdim,0,xdim,ydim)), mainSubLocal->digDisplayScale);
           cv::convertScaleAbs(mainSubLocal->combined, tempMat8bit(cv::Rect(2*xdim,0,xdim,ydim)), mainSubLocal->comDisplayScale);

           // tempMat.convertTo(tempMat8bit, CV_8U);
           // vidWrite.write(tempMat8bit);
            //tempMat8bit.convertTo(tempMat8bitc3,CV_8UC3);
           cv::resize(tempMat8bit,tempSized,cv::Size(),7,7);

            //vidWrite.write(tempMat8bit);
           vidWrite.write(tempSized);

            cv::imshow("output video",tempMat8bit);
            cv::waitKey(1);
        }

    }

    mainSubLocal->rawData->goToFrame(frameNumHold);
    mainSubLocal->updateDisplay();
    vidWrite.release();

}
