#include "sumframesdialog.h"
#include "ui_sumframesdialog.h"
#include "mainwindow.h"

SumFramesDialog::SumFramesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SumFramesDialog)
{
    ui->setupUi(this);
    mainSub = parent;
    tempMat = cv::Mat(128,128,CV_64F,cv::Scalar(0));
}

SumFramesDialog::~SumFramesDialog()
{
    delete ui;
}

void SumFramesDialog::on_pushButton_collectVals_clicked()
{
    MainWindow *mainSubLocal = reinterpret_cast<MainWindow*>(mainSub);
    startFrame = ui->lineEdit_StartFrame->text().toUInt();
    endFrame = ui->lineEdit_EndFrame->text().toUInt();
    step = ui->lineEdit_Step->text().toUInt();
    uint numframes = mainSubLocal->rawData->getNumOfFrames();
    uint frameHold = mainSubLocal->rawData->getFrameNum();
    mainSubLocal->results = cv::Mat(128,128,CV_64F,cv::Scalar(0));
    double scaleFactor = mainSubLocal->getScaleFactor();

    if(endFrame >= numframes)
    {
            endFrame = numframes-1;
    }

    if(startFrame >= numframes)
    {
            startFrame = numframes-1;
    }


    if((endFrame>startFrame)&&(step))
    {

        for(uint i = startFrame; i<= endFrame;i=i+step)
        {
            mainSubLocal->rawData->goToFrame(i);
            //mainSubLocal->updateDisplay();


            if(ui->checkBox_darkSub->isChecked())
            {
                mainSubLocal->rawData->imgAnalog->convertTo(mainSubLocal->scaledAna,CV_64F);
                mainSubLocal->scaledAna -= mainSubLocal->dark;
                mainSubLocal->rawData->imgDigital->convertTo(mainSubLocal->scaledDig,CV_64F);
                mainSubLocal->combined = (mainSubLocal->scaledDig*(scaleFactor))+(mainSubLocal->scaledAna);
                mainSubLocal->rawData->imgGain->convertTo(mainSubLocal->scaledGain,CV_64F);

            }
            else
            {
                mainSubLocal->rawData->imgAnalog->convertTo(mainSubLocal->scaledAna,CV_64F);
                mainSubLocal->rawData->imgDigital->convertTo(mainSubLocal->scaledDig,CV_64F);
                mainSubLocal->combined = (mainSubLocal->scaledDig*(scaleFactor))+(mainSubLocal->scaledAna);
                mainSubLocal->rawData->imgGain->convertTo(mainSubLocal->scaledGain,CV_64F);
            }

            if(ui->radioButton_analog->isChecked())
            {
                tempMat = mainSubLocal->scaledAna;
                if(ui->checkBox_offSub->isChecked())
                {
                    tempMat = tempMat - mainSubLocal->offsets[i];
                }

                if(ui->checkBox_thresh->isChecked())
                {
                    cv::threshold(tempMat,tempMat,ui->lineEdit_thresh->text().toDouble(),1,3);
                }

                mainSubLocal->results += tempMat;

            }
            else if(ui->radioButton_digital->isChecked())
            {
                tempMat = mainSubLocal->scaledDig;
                mainSubLocal->results += tempMat;

            }
            else if(ui->radioButton_cobined->isChecked())
            {
                tempMat = mainSubLocal->combined;
                if(ui->checkBox_offSub->isChecked())
                {
                    tempMat = tempMat - mainSubLocal->offsets[i];
                }

                if(ui->checkBox_thresh->isChecked())
                {
                    cv::threshold(tempMat,tempMat,ui->lineEdit_thresh->text().toDouble(),1,3);
                }

            }


        }
    }

    mainSubLocal->rawData->goToFrame(frameHold);
    mainSubLocal->updateDisplay();
}
