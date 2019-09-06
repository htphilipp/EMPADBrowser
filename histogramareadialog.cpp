#include "histogramareadialog.h"
#include "ui_histogramareadialog.h"
#include "mainwindow.h"


HistogramAreaDialog::HistogramAreaDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HistogramAreaDialog)
{
    ui->setupUi(this);
    mainSub = parent;
    ui->histPlot->addGraph();
}

HistogramAreaDialog::~HistogramAreaDialog()
{
    delete ui;
}

void HistogramAreaDialog::on_pushButton_collectVals_clicked()
{
    MainWindow *mainSubLocal = reinterpret_cast<MainWindow*>(mainSub);
    pixelDat.clear();

    int x1 = ui->lineEdit_pixX1->text().toInt();
    int y1 = ui->lineEdit_pixY1->text().toInt();
    int x2 = ui->lineEdit_pixX2->text().toInt();
    int y2 = ui->lineEdit_pixY2->text().toInt();

    int xlow,ylow,xhigh,yhigh;

    int x,y;

    if(x1>x2)
    {
        xhigh = x1;
        xlow = x2;
    }
    else
    {
        xhigh = x2;
        xlow = x1;
    }

    if(y1>y2)
    {
       yhigh = y1;
       ylow = y2;
    }
    else
    {
        yhigh = y2;
        ylow = y1;
    }

    int xdim = int(mainSubLocal->rawData->getxdim());
    int ydim = int(mainSubLocal->rawData->getydim());
    double scaleFactor;

    if(xhigh>=xdim)
    {
        xhigh = xdim-1;
    }
    if(xlow>=xdim)
    {
        xlow = xdim -1;
    }

    if(yhigh>=ydim)
    {
        yhigh = ydim-1;
    }
    if(ylow>=ydim)
    {
        ylow = ydim-1;
    }

    if(xhigh<0)
    {
        xhigh = 0;
    }
    if(xlow<0)
    {
        xlow = 0;
    }
    if(yhigh<0)
    {
        yhigh = 0;
    }
    if(ylow<0)
    {
        ylow = 0;
    }


    double minval, maxval;

    int minframe = ui->lineEdit_StartFrame->text().toInt();
    int maxframe = ui->lineEdit_EndFrame->text().toInt();

    uint InitialFrame = mainSubLocal->rawData->getFrameNum();

    if(minframe<0) {minframe = 0; ui->lineEdit_StartFrame->text().setNum(0);}
    if(maxframe>= int(mainSubLocal->rawData->getNumOfFrames())) {maxframe = int(mainSubLocal->rawData->getNumOfFrames())-1; ui->lineEdit_EndFrame->text().setNum(maxframe);}


    for(auto i = minframe; i<=maxframe; i=i+(ui->lineEdit_Step->text().toInt()) )
    {
        mainSubLocal->rawData->goToFrame(uint(i));
        for(x = xlow;x<=xhigh;x++)
        {
            for( y = ylow;y<=yhigh;y++)
            {
                //mainSubLocal->rawData->goToFrame(uint(i));
                if(ui->radioButton_Analog->isChecked())
                {
                    if(!(ui->checkBox_backSub->isChecked())) // not background subtracted
                    {
                        mainSubLocal->rawData->imgAnalog->convertTo(mainSubLocal->scaledAna,CV_64F);
                        pixelDat.push_back(mainSubLocal->scaledAna.at<double>(y,x));
                    }
                    else //background subtracted
                    {
                        mainSubLocal->rawData->imgAnalog->convertTo(mainSubLocal->scaledAna,CV_64F);
                        pixelDat.push_back((mainSubLocal->scaledAna.at<double>(y,x))-(mainSubLocal->dark.at<double>(y,x)));
                    }
                }
                else if(ui->radioButton_Combined->isChecked())
                {
                    if(!(ui->checkBox_backSub->isChecked())) // not background subtracted
                    {
                        scaleFactor = mainSubLocal->getScaleFactor();
                        mainSubLocal->rawData->imgDigital->convertTo(mainSubLocal->scaledDig,CV_64F);
                        mainSubLocal->rawData->imgAnalog->convertTo(mainSubLocal->scaledAna,CV_64F);
                        pixelDat.push_back((mainSubLocal->scaledDig.at<double>(y,x))*scaleFactor + (mainSubLocal->scaledAna.at<double>(y,x)));

                    }
                    else //background subtracted
                    {
                        scaleFactor = mainSubLocal->getScaleFactor();
                        mainSubLocal->rawData->imgDigital->convertTo(mainSubLocal->scaledDig,CV_64F);
                        mainSubLocal->rawData->imgAnalog->convertTo(mainSubLocal->scaledAna,CV_64F);
                        pixelDat.push_back((mainSubLocal->scaledDig.at<double>(y,x))*scaleFactor + (mainSubLocal->scaledAna.at<double>(y,x))-(mainSubLocal->dark.at<double>(y,x)));
                    }
                }
                else
                {
                    //digital default
                    mainSubLocal->rawData->imgDigital->convertTo(mainSubLocal->scaledDig,CV_64F);
                    pixelDat.push_back(mainSubLocal->scaledDig.at<double>(y,x));
                }

                if(i==minframe)
                {
                    minval = pixelDat.back();
                    maxval = pixelDat.back();
                }
                else
                {
                    if(pixelDat.back()<minval)
                    {
                        minval = pixelDat.back();
                    }

                    if(pixelDat.back()>maxval)
                    {
                        maxval = pixelDat.back();
                    }
                }
            }
        }
    }

    ui->label_PixelMinVal->setNum(minval);
    ui->label_PixelsMaxVal->setNum(maxval);

    mainSubLocal->rawData->goToFrame(InitialFrame);
}



void HistogramAreaDialog::on_pushButton_MakeBinPlot_clicked()
{
    double binstart = ui->lineEdit_BinStart->text().toDouble();
    double binstop = ui->lineEdit_BinStop->text().toDouble();
    double binstep = ui->lineEdit_NumOfBins->text().toDouble();


    int calcbin;
    histBins.clear();
    binVals.clear();

    if(pixelDat.size()>1)
    {
        for(auto i = binstart; i<binstop; i=i+binstep)
        {
            histBins.push_back(i+0.5*binstep);
            binVals.push_back(0);
        }
        for(auto pd:pixelDat)
        {
            calcbin = int(std::floor((pd-binstart)/binstep));
            if((calcbin>0) && (calcbin<int(binVals.size())))
            {
                binVals[(calcbin)] = binVals[(calcbin)]+1;
            }
        }

        ui->histPlot->graph(0)->setData(histBins,binVals);
        ui->histPlot->xAxis->rescale();
        ui->histPlot->yAxis->rescale();
        ui->histPlot->replot();

    }
}

void HistogramAreaDialog::on_checkBox_stateChanged(int arg1)
{
    if(arg1)
    {
        ui->histPlot->yAxis->setScaleType(QCPAxis::stLogarithmic);
    }
    else
    {
       ui->histPlot->yAxis->setScaleType(QCPAxis::stLinear);
    }
    ui->histPlot->replot();
}

void HistogramAreaDialog::on_pushButton_expData_clicked()
{
    QString filenameToSave= QFileDialog::getSaveFileName(this,tr("File to save data to"), "c:/", tr("csv Files (*.csv)"));

    QFile output(filenameToSave);
    output.open(QFile::WriteOnly|QFile::Truncate|QIODevice::Text);
    QTextStream stream(&output);
    stream<<"bin center , "<<" populaton"<<"\n";
    for(auto i = 0; i<histBins.length();i++)
    {
        stream<<histBins[i]<<","<<binVals[i]<<"\n";
    }
    output.close();
}
