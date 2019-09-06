#include "histogramdialog.h"
#include "ui_histogramdialog.h"
#include "mainwindow.h"

HistogramDialog::HistogramDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HistogramDialog)
{
    ui->setupUi(this);
    mainSub = parent;
    ui->histPlot->addGraph();

}

HistogramDialog::~HistogramDialog()
{
    delete ui;
}

void HistogramDialog::on_pushButton_collectVals_clicked()
{
    MainWindow *mainSubLocal = reinterpret_cast<MainWindow*>(mainSub);
    pixelDat.clear();

    int x = ui->lineEdit_PixX->text().toInt();
    int y = ui->lineEdit_pixY->text().toInt();
    int xdim = int(mainSubLocal->rawData->getxdim());
    int ydim = int(mainSubLocal->rawData->getydim());
    double scaleFactor;

    double minval, maxval;

    int minframe = ui->lineEdit_StartFrame->text().toInt();
    int maxframe = ui->lineEdit_EndFrame->text().toInt();

    uint InitialFrame = mainSubLocal->rawData->getFrameNum();

    if(minframe<0) {minframe = 0; ui->lineEdit_StartFrame->text().setNum(0);}
    if(maxframe>= int(mainSubLocal->rawData->getNumOfFrames())) {maxframe = int(mainSubLocal->rawData->getNumOfFrames())-1; ui->lineEdit_EndFrame->text().setNum(maxframe);}

/*************************************Old Routine

    for(auto i = minframe; i<=maxframe; i=i+(ui->lineEdit_Step->text().toInt()) )
    {
        mainSubLocal->rawData->goToFrame(uint(i));
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
**********************************************NEW hopefully more efficient code****/
    for(auto i = minframe; i<=maxframe; i=i+(ui->lineEdit_Step->text().toInt()) )
    {
        if(ui->radioButton_Analog->isChecked())
        {
            if(!(ui->checkBox_backSub->isChecked())) // not background subtracted
            {
                pixelDat.push_back(double(mainSubLocal->rawData->getAnalog(uint(y),uint(x),uint(i))));
            }
            else //background subtracted
            {
                if(!(ui->checkBox_offSub->isChecked()) || (mainSubLocal->rawData->getNumOfFrames()!=uint(mainSubLocal->offsets.size())))
                {
                    pixelDat.push_back(double(mainSubLocal->rawData->getAnalog(uint(y),uint(x),uint(i)))-(mainSubLocal->dark.at<double>(y,x)));
                }
                else
                {
                    pixelDat.push_back(double(mainSubLocal->rawData->getAnalog(uint(y),uint(x),uint(i)))-(mainSubLocal->dark.at<double>(y,x))-(mainSubLocal->offsets[uint(i)]));
                }
            }
        }
        else if(ui->radioButton_Combined->isChecked())
        {
            scaleFactor = mainSubLocal->getScaleFactor();
            if(!(ui->checkBox_backSub->isChecked())) // not background subtracted
            {
                pixelDat.push_back(scaleFactor*double(mainSubLocal->rawData->getDigital(uint(y),uint(x),uint(i))) + double(mainSubLocal->rawData->getAnalog(uint(y),uint(x),uint(i))));
            }
            else
            {
                if(!(ui->checkBox_offSub->isChecked()) || (mainSubLocal->rawData->getNumOfFrames()!=uint(mainSubLocal->offsets.size())))
                {
                    pixelDat.push_back(scaleFactor*double(mainSubLocal->rawData->getDigital(uint(y),uint(x),uint(i))) + double(mainSubLocal->rawData->getAnalog(uint(y),uint(x),uint(i)))-(mainSubLocal->dark.at<double>(y,x)));
                }
                else
                {
                    pixelDat.push_back(scaleFactor*double(mainSubLocal->rawData->getDigital(uint(y),uint(x),uint(i))) + double(mainSubLocal->rawData->getAnalog(uint(y),uint(x),uint(i)))-(mainSubLocal->dark.at<double>(y,x))-(mainSubLocal->offsets[uint(i)]));
                }
            }

        }
        else
        {
             pixelDat.push_back(double(mainSubLocal->rawData->getDigital(uint(y),uint(x),uint(i))));
        }



/********/
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

    ui->label_PixelMinVal->setNum(minval);
    ui->label_PixelsMaxVal->setNum(maxval);

    mainSubLocal->rawData->goToFrame(InitialFrame);
}

void HistogramDialog::on_pushButton_MakeBinPlot_clicked()
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

void HistogramDialog::on_pushButton_expData_clicked()
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

void HistogramDialog::on_pushButton_clicked()
{
    int x1 = ui->lineEdit_x1->text().toInt();
    int y1 = ui->lineEdit_y1->text().toInt();
    int x2 = ui->lineEdit_x2->text().toInt();
    int y2 = ui->lineEdit_y2->text().toInt();
    QVector<double> pixel;

    int xmin, ymin, xmax, ymax;


    if(x1<x2)
    {
        xmin = x1;
        xmax = x2;
    }
    else
    {
        xmin = x2;
        xmax = x1;
    }

    if(y1<y2)
    {
        ymin = y1;
        ymax = y2;
    }
    else
    {
        ymin = y2;
        ymax = y1;
    }

    for(auto i = xmin; i<= xmax; i++)
    {
        for(auto j = ymin; j <= ymax; j++)
        {
            pixel.clear();
            ui->lineEdit_PixX->setText(QString::number(i));
            ui->lineEdit_pixY->setText(QString::number(j));
            on_pushButton_collectVals_clicked();
            on_pushButton_MakeBinPlot_clicked();

            pixel.push_back(i);
            pixel.push_back(j);

            pixCoords.push_back(pixel);
            multiPixDat.push_back(binVals);
        }
    }

    QString filenameToSave= QFileDialog::getSaveFileName(this,tr("File to save data to"), "c:/", tr("csv Files (*.csv)"));

    QFile output(filenameToSave);
    output.open(QFile::WriteOnly|QFile::Truncate|QIODevice::Text);
    QTextStream stream(&output);

    stream<<"bin centers, ";
    for(auto i = 0; i<pixCoords.size();i++)
    {
        stream<<"x"<<pixCoords[i][0]<<"y"<<pixCoords[i][1];
        if(i<(pixCoords.size()-1))
        {
            stream<<",";
        }
    }
    stream<<"\n";

    for(auto i = 0; i<histBins.size();i++)
    {
        stream<<histBins[i]<<",";
        for( auto j = 0; j<multiPixDat.size();j++)
        {            stream<<multiPixDat[j][i];
            if(j < (multiPixDat.size()-1))
            {
                stream<<",";
            }
        }
        stream<<"\n";
    }

    output.close();
}
