#include "plotpixels.h"
#include "ui_plotpixels.h"
#include "mainwindow.h"

PlotPixels::PlotPixels(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlotPixels)
{
    ui->setupUi(this);
    mainSub = parent;
    ui->pixelPlot->addGraph();
    ui->pixelPlot->addGraph();
}

PlotPixels::~PlotPixels()
{
    delete ui;
}

void PlotPixels::on_pushButton_collectVals_clicked()
{
    MainWindow *mainSubLocal = reinterpret_cast<MainWindow*>(mainSub);
    int x = ui->lineEdit_PixX->text().toInt();
    int y = ui->lineEdit_pixY->text().toInt();
    int xdim = int(mainSubLocal->rawData->getxdim());
    int ydim = int(mainSubLocal->rawData->getydim());
    double scaleFactor;

    xpix = x;
    ypix = y;

    oddframenum.clear();
    oddFrames.clear();
    evenframenum.clear();
    evenFrames.clear();

    double minval, maxval;
    double minvale,maxvale;
    double minvalo,maxvalo;

    int minframe = ui->lineEdit_StartFrame->text().toInt();
    int maxframe = ui->lineEdit_EndFrame->text().toInt();

     uint InitialFrame = mainSubLocal->rawData->getFrameNum();
     if(minframe<0) {minframe = 0; ui->lineEdit_StartFrame->text().setNum(0);}
     if(maxframe>= int(mainSubLocal->rawData->getNumOfFrames())) {maxframe = int(mainSubLocal->rawData->getNumOfFrames())-1; ui->lineEdit_EndFrame->text().setNum(maxframe);}

     for(auto i = minframe; i<=maxframe; i=i++ )
     {
         mainSubLocal->rawData->goToFrame(uint(i));
         if(ui->radioButton_Analog->isChecked())
         {
             if(!(ui->checkBox_backSub->isChecked())) // not background subtracted
             {
                 mainSubLocal->rawData->imgAnalog->convertTo(mainSubLocal->scaledAna,CV_64F);
                 if(i%2)
                 {
                    oddFrames.push_back(mainSubLocal->scaledAna.at<double>(y,x));
                    oddframenum.push_back(double(i));
                 }
                 else
                 {
                    evenFrames.push_back(mainSubLocal->scaledAna.at<double>(y,x));
                    evenframenum.push_back(double(i));
                 }
             }
             else //background subtracted
             {
                 mainSubLocal->rawData->imgAnalog->convertTo(mainSubLocal->scaledAna,CV_64F);
                 //pixelDat.push_back((mainSubLocal->scaledAna.at<double>(y,x))-(mainSubLocal->dark.at<double>(y,x)));
                 if(i%2)
                 {
                    oddFrames.push_back((mainSubLocal->scaledAna.at<double>(y,x))-(mainSubLocal->dark.at<double>(y,x)));
                    oddframenum.push_back(double(i));
                 }
                 else
                 {
                    evenFrames.push_back((mainSubLocal->scaledAna.at<double>(y,x))-(mainSubLocal->dark.at<double>(y,x)));
                    evenframenum.push_back(double(i));
                 }
             }
         }
         else if(ui->radioButton_Combined->isChecked())
         {
             if(!(ui->checkBox_backSub->isChecked())) // not background subtracted
             {
                 scaleFactor = mainSubLocal->getScaleFactor();
                 mainSubLocal->rawData->imgDigital->convertTo(mainSubLocal->scaledDig,CV_64F);
                 mainSubLocal->rawData->imgAnalog->convertTo(mainSubLocal->scaledAna,CV_64F);
                 //pixelDat.push_back((mainSubLocal->scaledDig.at<double>(y,x))*scaleFactor + (mainSubLocal->scaledAna.at<double>(y,x)));
                 if(i%2)
                 {
                    oddFrames.push_back((mainSubLocal->scaledDig.at<double>(y,x))*scaleFactor + (mainSubLocal->scaledAna.at<double>(y,x)));
                    oddframenum.push_back(double(i));
                 }
                 else
                 {
                    evenFrames.push_back((mainSubLocal->scaledDig.at<double>(y,x))*scaleFactor + (mainSubLocal->scaledAna.at<double>(y,x)));
                    evenframenum.push_back(double(i));
                 }

             }
             else //background subtracted
             {
                 scaleFactor = mainSubLocal->getScaleFactor();
                 mainSubLocal->rawData->imgDigital->convertTo(mainSubLocal->scaledDig,CV_64F);
                 mainSubLocal->rawData->imgAnalog->convertTo(mainSubLocal->scaledAna,CV_64F);
                 //pixelDat.push_back((mainSubLocal->scaledDig.at<double>(y,x))*scaleFactor + (mainSubLocal->scaledAna.at<double>(y,x))-(mainSubLocal->dark.at<double>(y,x)));
                 if(i%2)
                 {
                    oddFrames.push_back((mainSubLocal->scaledDig.at<double>(y,x))*scaleFactor + (mainSubLocal->scaledAna.at<double>(y,x))-(mainSubLocal->dark.at<double>(y,x)));
                    oddframenum.push_back(double(i));
                 }
                 else
                 {
                    evenFrames.push_back((mainSubLocal->scaledDig.at<double>(y,x))*scaleFactor + (mainSubLocal->scaledAna.at<double>(y,x))-(mainSubLocal->dark.at<double>(y,x)));
                    evenframenum.push_back(double(i));
                 }
             }
         }
         else
         {
             //digital default
             mainSubLocal->rawData->imgDigital->convertTo(mainSubLocal->scaledDig,CV_64F);
             //pixelDat.push_back(mainSubLocal->scaledDig.at<double>(y,x));
             mainSubLocal->rawData->imgAnalog->convertTo(mainSubLocal->scaledAna,CV_64F);
             if(i%2)
             {
                oddFrames.push_back(mainSubLocal->scaledDig.at<double>(y,x));
                oddframenum.push_back(double(i));
             }
             else
             {
                evenFrames.push_back(mainSubLocal->scaledDig.at<double>(y,x));
                evenframenum.push_back(double(i));
             }
         }
    }

     ui->pixelPlot->graph(0)->setData(evenframenum,evenFrames);
     ui->pixelPlot->graph(0)->setPen(QPen(Qt::red));
     ui->pixelPlot->graph(1)->setData(oddframenum,oddFrames);
     ui->pixelPlot->graph(1)->setPen(QPen(Qt::blue));
     ui->pixelPlot->xAxis->rescale();
     ui->pixelPlot->yAxis->rescale();
     ui->pixelPlot->graph(0)->setVisible(ui->checkBox_showEven->isChecked());
     ui->pixelPlot->graph(1)->setVisible(ui->checkBox_showOdd->isChecked());

     ui->pixelPlot->replot();
}

void PlotPixels::on_checkBox_showEven_stateChanged(int arg1)
{
    ui->pixelPlot->graph(0)->setVisible(ui->checkBox_showEven->isChecked());
    ui->pixelPlot->graph(1)->setVisible(ui->checkBox_showOdd->isChecked());

    ui->pixelPlot->replot();
}

void PlotPixels::on_checkBox_showOdd_stateChanged(int arg1)
{
    ui->pixelPlot->graph(0)->setVisible(ui->checkBox_showEven->isChecked());
    ui->pixelPlot->graph(1)->setVisible(ui->checkBox_showOdd->isChecked());

    ui->pixelPlot->replot();
}

void PlotPixels::on_pushButton_expData_clicked()
{
    QString filenameToSave= QFileDialog::getSaveFileName(this,tr("File to save data to"), "c:/", tr("csv Files (*.csv)"));

    QFile output(filenameToSave);
    output.open(QFile::WriteOnly|QFile::Truncate|QIODevice::Text);
    QTextStream stream(&output);
    stream<<"pixel x = "<< xpix<<" y = "<< ypix<<"\n";
    stream<<"odd frame number , "<<" pixel output "<<"\n";
    for(auto i = 0; i<oddframenum.length();i++)
    {
        stream<<oddframenum[i]<<","<<oddFrames[i]<<"\n";
    }
    stream<<"even frame number , "<<" pixel output "<<"\n";
    for(auto i = 0; i<evenframenum.length();i++)
    {
        stream<<evenframenum[i]<<","<<evenFrames[i]<<"\n";
    }
    output.close();
}
