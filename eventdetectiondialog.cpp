#include "eventdetectiondialog.h"
#include "ui_eventdetectiondialog.h"
#include "mainwindow.h"

EventDetectionDialog::EventDetectionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EventDetectionDialog)
{
    ui->setupUi(this);
    mainSub = parent;
    ui->histPlot->addGraph();
}

EventDetectionDialog::~EventDetectionDialog()
{
    delete ui;
}

void EventDetectionDialog::on_checkBox_stateChanged(int arg1)
{
    MainWindow *mainSubLocal = reinterpret_cast<MainWindow*>(mainSub);
    if(ui->checkBox->isChecked())
    {
       mainSubLocal->evDetect = 1;
    }
    else
    {
        mainSubLocal->evDetect = 0;
    }

    mainSubLocal->updateDisplay();
    mainSubLocal->updatePixelValue();
}

void EventDetectionDialog::on_lineEdit_thresh_textChanged(const QString &arg1)
{
    MainWindow *mainSubLocal = reinterpret_cast<MainWindow*>(mainSub);
    mainSubLocal->evThresh = ui->lineEdit_thresh->text().toDouble();
    mainSubLocal->updateDisplay();
    mainSubLocal->updatePixelValue();
}

void EventDetectionDialog::on_pushButton_clicked()
{
    MainWindow *mainSubLocal = reinterpret_cast<MainWindow*>(mainSub);
    cv::Mat stat = mainSubLocal->evStats;
    cv::Mat centroid = mainSubLocal->evCentroids;
    int x,y,w,h;
    int xi,yi;
    double sum;
    ui->textBrowser->clear();
    int pad = ui->lineEdit_pad->text().toInt();
    for(auto i = 1; i<stat.rows;i++)
    {
        sum = 0;
            x = stat.at<int>(cv::Point(0, i))-pad;
            y = stat.at<int>(cv::Point(1, i))-pad;
            w = stat.at<int>(cv::Point(2, i))+2*pad;
            h = stat.at<int>(cv::Point(3, i))+2*pad;
            for(xi = x; xi<=(x+w);xi++)
            {
                for(yi = y; yi<=(y+h);yi++)
                {
                    sum +=mainSubLocal->combined.at<double>(yi,xi);
                }
            }
            ui->textBrowser->append(QString::number(sum));
            //ui->textBrowser->append(QString("\n"));

    }
}

void EventDetectionDialog::on_pushButton_2_clicked()
{
    MainWindow *mainSubLocal = reinterpret_cast<MainWindow*>(mainSub);
    if(!(ui->checkBox->isChecked()))
    {
        return;
    }

    double binstart = ui->lineEdit_binStart->text().toDouble();
    double binstop = ui->lineEdit_binStop->text().toDouble();
    double binstep = ui->lineEdit_binStep->text().toDouble();
    uint frameStart= ui->lineEdit_frameStart->text().toUInt();
    uint frameEnd = ui->lineEdit_frameEnd->text().toUInt();

    int x1 = ui->lineEdit_pixX1->text().toInt();
    int x2 = ui->lineEdit_pixX2->text().toInt();
    int y1 = ui->lineEdit_pixY1->text().toInt();
    int y2 = ui->lineEdit_pixY2->text().toInt();

    if(x1>=x2) return;
    if(y1>=y2) return;
    if(binstart>=binstop) return;
    if(frameStart>=frameEnd) return;

    int x,y,w,h;
    int xi,yi;
    double sum;
    int pad = ui->lineEdit_pad->text().toInt();
    int calcbin;
    histBins.clear();
    binVals.clear();
    uint frame;
    uint frameStep;

    frameStep = ui->lineEdit_frameStep->text().toUInt();

    uint preFrame = mainSubLocal->rawData->getFrameNum();
    for(auto i = binstart; i<binstop; i=i+binstep)
    {
        histBins.push_back(i+0.5*binstep);
        binVals.push_back(0);
    }

    QProgressDialog progress("Histogramming events...","cancel",0, int(frameEnd-frameStart));
    progress.setWindowModality(Qt::WindowModal);

    for(frame=frameStart; frame<=frameEnd; frame=frame+frameStep)
    {
        mainSubLocal->rawData->goToFrame(frame);
        mainSubLocal->updateDisplay();
        for(auto i = 1; i<mainSubLocal->evStats.rows;i++)
        {
            //sum = 0;
                x = mainSubLocal->evStats.at<int>(cv::Point(0, i))-pad;
                y = mainSubLocal->evStats.at<int>(cv::Point(1, i))-pad;
                w = mainSubLocal->evStats.at<int>(cv::Point(2, i))+2*pad;
                h = mainSubLocal->evStats.at<int>(cv::Point(3, i))+2*pad;

                if(x>=x1 && ((x+w+2*pad)<=x2) && y>=y1 && ((y+h+pad)<=y2))
                {
                    sum = 0;
                    for(xi = x; xi<=(x+w);xi++)
                    {
                        for(yi = y; yi<=(y+h);yi++)
                        {
                            sum +=mainSubLocal->combined.at<double>(yi,xi);
                        }
                    }
                    calcbin = int(std::floor((sum-binstart)/binstep));
                    if((calcbin>0) && (calcbin<int(binVals.size())))
                    {
                        binVals[(calcbin)] = binVals[(calcbin)]+1;
                    }
                }
        }
        progress.setValue(int(frame-frameStart));

    }
    ui->histPlot->graph(0)->setData(histBins,binVals);
    ui->histPlot->xAxis->rescale();
    ui->histPlot->yAxis->rescale();
    ui->histPlot->replot();
    mainSubLocal->rawData->goToFrame(preFrame);
    mainSubLocal->updateDisplay();

}

void EventDetectionDialog::on_pushButton_3_clicked()
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
