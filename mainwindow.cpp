#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    scaledAna = cv::Mat(128,128,CV_64F,cv::Scalar(0));
    scaledDig = cv::Mat(128,128,CV_64F,cv::Scalar(0));
    combined = cv::Mat(128,128,CV_64F,cv::Scalar(0));
    dark = cv::Mat(128,128,CV_64F,cv::Scalar(0));
    scaledGain = cv::Mat(128,128,CV_64F,cv::Scalar(0));
    results = cv::Mat(128,128,CV_64F,cv::Scalar(0));

    darks[0]    =   cv::Mat(128,128,CV_64F,cv::Scalar(0));
    darks[1]    =   cv::Mat(128,128,CV_64F,cv::Scalar(0));
    lgOffset[0] =   cv::Mat(128,128,CV_64F,cv::Scalar(370));
    lgOffset[1] =   cv::Mat(128,128,CV_64F,cv::Scalar(370));
    dADUdN[0]   =   cv::Mat(128,128,CV_64F,cv::Scalar(5900));
    dADUdN[1]   =   cv::Mat(128,128,CV_64F,cv::Scalar(5900));
    sRatio[0]   =   cv::Mat(128,128,CV_64F,cv::Scalar(19.4));
    sRatio[1]   =   cv::Mat(128,128,CV_64F,cv::Scalar(19.4));

    gainMask = cv::Mat(128,128,CV_64F,cv::Scalar(0));
    notgainMask = cv::Mat(128,128,CV_64F,cv::Scalar(0));

    cv::namedWindow("analog", cv::WINDOW_NORMAL);
    cv::namedWindow("digital", cv::WINDOW_NORMAL);
    cv::namedWindow("combined",cv::WINDOW_NORMAL);
    cv::namedWindow("gain",cv::WINDOW_NORMAL);
    cv::namedWindow("results",cv::WINDOW_NORMAL);


    cv::setMouseCallback("analog",&MainWindow::onMouseAna,this);
    cv::setMouseCallback("digital",&MainWindow::onMouseDig,this);
    cv::setMouseCallback("combined",&MainWindow::onMouseComb,this);
    cv::setMouseCallback("results",&MainWindow::onMouseResults,this);
    cv::setMouseCallback("gain",&MainWindow::onMouseGain,this);

    rawData = nullptr;
    rawBGData = nullptr;
    histui = new HistogramDialog(this);
    pltPix = new PlotPixels(this);
    histAreaui = new HistogramAreaDialog(this);
    movDialog = new movieDialog(this);
    calcOffsets = new CalcDebounceOffsets(this);
    sumFrames = new SumFramesDialog(this);
    offsets.clear();
    mousex = 0;
    mousey = 0;

    anaDisplayScale = ui->lineEdit_4->text().toDouble();
    digDisplayScale = ui->lineEdit_5->text().toDouble();
    comDisplayScale = ui->lineEdit_6->text().toDouble();
}

void MainWindow::onMouseAna(int evt, int x, int y, int flags, void *param)
{
    MainWindow *mainSub = (MainWindow*) param;  //using param pointer to "avoid" restrictions on static member functions.

    mainSub->ui->label_13->setNum(x);
    mainSub->ui->label_15->setNum(y);
    //mainSub->ui->label_17->setNum(mainSub->scaledAna.at<double>(y,x));
    mainSub->ui->label_anaPix->setNum(mainSub->scaledAna.at<double>(y,x));
    mainSub->ui->label_digPix->setNum(mainSub->scaledDig.at<double>(y,x));
    mainSub->ui->label_comPix->setNum(mainSub->combined.at<double>(y,x));
    mainSub->ui->label_gainPix->setNum(mainSub->scaledGain.at<double>(y,x));
    mainSub->ui->label_resPix->setNum(mainSub->results.at<double>(y,x));
    mainSub->mousex = x;
    mainSub->mousey = y;

    mainSub->ui->label_18->setText("analog");
}

void MainWindow::onMouseDig(int evt, int x, int y, int flags, void *param)
{
    MainWindow *mainSub = (MainWindow*) param;  //using param pointer to "avoid" restrictions on static member functions.

    mainSub->ui->label_13->setNum(x);
    mainSub->ui->label_15->setNum(y);
   // mainSub->ui->label_17->setNum(mainSub->scaledDig.at<double>(y,x));
    mainSub->ui->label_anaPix->setNum(mainSub->scaledAna.at<double>(y,x));
    mainSub->ui->label_digPix->setNum(mainSub->scaledDig.at<double>(y,x));
    mainSub->ui->label_comPix->setNum(mainSub->combined.at<double>(y,x));
    mainSub->ui->label_gainPix->setNum(mainSub->scaledGain.at<double>(y,x));
    mainSub->ui->label_resPix->setNum(mainSub->results.at<double>(y,x));
    mainSub->mousex = x;
    mainSub->mousey = y;

    mainSub->ui->label_18->setText("digital");
}

void MainWindow::onMouseComb(int evt, int x, int y, int flags, void *param)
{
    MainWindow *mainSub = (MainWindow*) param;  //using param pointer to "avoid" restrictions on static member functions.

    mainSub->ui->label_13->setNum(x);
    mainSub->ui->label_15->setNum(y);
   // mainSub->ui->label_17->setNum(mainSub->combined.at<double>(y,x));
    mainSub->ui->label_anaPix->setNum(mainSub->scaledAna.at<double>(y,x));
    mainSub->ui->label_digPix->setNum(mainSub->scaledDig.at<double>(y,x));
    mainSub->ui->label_comPix->setNum(mainSub->combined.at<double>(y,x));
    mainSub->ui->label_gainPix->setNum(mainSub->scaledGain.at<double>(y,x));
    mainSub->ui->label_resPix->setNum(mainSub->results.at<double>(y,x));
    mainSub->mousex = x;
    mainSub->mousey = y;

    mainSub->ui->label_18->setText("combined");
}

void MainWindow::onMouseResults(int evt, int x, int y, int flags, void *param)
{
    MainWindow *mainSub = (MainWindow*) param;  //using param pointer to "avoid" restrictions on static member functions.

    mainSub->ui->label_13->setNum(x);
    mainSub->ui->label_15->setNum(y);
   // mainSub->ui->label_17->setNum(mainSub->combined.at<double>(y,x));
    mainSub->ui->label_anaPix->setNum(mainSub->scaledAna.at<double>(y,x));
    mainSub->ui->label_digPix->setNum(mainSub->scaledDig.at<double>(y,x));
    mainSub->ui->label_comPix->setNum(mainSub->combined.at<double>(y,x));
    mainSub->ui->label_gainPix->setNum(mainSub->scaledGain.at<double>(y,x));
    mainSub->ui->label_resPix->setNum(mainSub->results.at<double>(y,x));
    mainSub->mousex = x;
    mainSub->mousey = y;

    mainSub->ui->label_18->setText("results");
}

void MainWindow::onMouseGain(int evt, int x, int y, int flags, void *param)
{
    MainWindow *mainSub = (MainWindow*) param;  //using param pointer to "avoid" restrictions on static member functions.

    mainSub->ui->label_13->setNum(x);
    mainSub->ui->label_15->setNum(y);
   // mainSub->ui->label_17->setNum(mainSub->combined.at<double>(y,x));
    mainSub->ui->label_anaPix->setNum(mainSub->scaledAna.at<double>(y,x));
    mainSub->ui->label_digPix->setNum(mainSub->scaledDig.at<double>(y,x));
    mainSub->ui->label_comPix->setNum(mainSub->combined.at<double>(y,x));
    mainSub->ui->label_gainPix->setNum(mainSub->scaledGain.at<double>(y,x));
    mainSub->ui->label_resPix->setNum(mainSub->results.at<double>(y,x));
    mainSub->mousex = x;
    mainSub->mousey = y;

    mainSub->ui->label_18->setText("gain");
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    rawfilename = QFileDialog::getOpenFileName(this,
             tr("Open raw data file"), "c:/", tr("raw Files (*.raw)"));
    ui->lineEdit->setText(rawfilename);

}

void MainWindow::on_lineEdit_editingFinished()
{
    rawfilename = ui->lineEdit->text();
}

void MainWindow::on_pushButton_2_clicked()
{
    rawBGfilename = QFileDialog::getOpenFileName(this,
             tr("Open raw data file"), "c:/", tr("raw Files (*.raw)"));
    ui->lineEdit_2->setText(rawBGfilename);
}

void MainWindow::on_lineEdit_2_editingFinished()
{
    rawBGfilename = ui->lineEdit_2->text();
}

void MainWindow::on_lineEdit_textChanged(const QString &arg1)
{
    rawfilename = ui->lineEdit->text();
}

void MainWindow::on_lineEdit_2_textChanged(const QString &arg1)
{
    rawBGfilename = ui->lineEdit_2->text();
}


void MainWindow::on_pushButton_3_clicked()
{
   //delete rawData;
   //delete rawBGData;

   if(boost::filesystem::exists(rawfilename.toStdString()))
   {
        delete rawData;
        rawData = new Empad2(128,128,rawfilename.toStdString());
        ui->numFrames->setNum(int(rawData->getNumOfFrames()));
        rawData->goToFrame(100);
        ui->frameSlider->setRange(0,int(rawData->getNumOfFrames()));
        ui->frameSlider->setValue(int(rawData->getFrameNum()));
        ui->label_3->setNum(int(rawData->getFrameNum()));
   }
   else
   {
       ui->label_3->setText(QString("Data File Not Found"));
   }

   if(boost::filesystem::exists(rawBGfilename.toStdString()))
   {
        delete rawBGData;
        rawBGData = new Empad2(128,128,rawBGfilename.toStdString());
        ui->label_9->setNum(int(rawBGData->getNumOfFrames()));
   }
   else
   {
       ui->label_9->setText(QString("Dark File Not Found"));
   }


   //ui->numFrames->setNum(int(rawData->getNumOfFrames()));
   //rawData->goToFrame(100);
   //ui->frameSlider->setRange(0,int(rawData->getNumOfFrames()));
   //ui->frameSlider->setValue(int(rawData->getFrameNum()));
   //ui->label_3->setNum(int(rawData->getFrameNum()));
   //ui->label_9->setNum(int(rawBGData->getNumOfFrames()));

   if(rawData!=nullptr)
   {
        updateDisplay();
   }

}

void MainWindow::updateDisplay()
{
    double scaleFactor;
    int except;
    uint frame;
    int cI; //calibration index
    ui->label_20->setText(QString(""));
    frame = rawData->getFrameNum();

    if(!(ui->checkBox_2->isChecked()) )
    {
        if(!(ui->checkBox->isChecked()) && !(ui->checkBox_calib->isChecked()))
        {
            rawData->imgAnalog->convertTo(scaledAna,CV_64F);
            cv::convertScaleAbs(scaledAna-8000, adjMap, anaDisplayScale);
            cv::imshow("analog", adjMap);

            rawData->imgDigital->convertTo(scaledDig,CV_64F);
            cv::convertScaleAbs(*(rawData->imgDigital), adjMap, digDisplayScale);
            cv::imshow("digital", adjMap);

            scaleFactor = ui->lineEdit_3->text().toInt();
            rawData->imgDigital->convertTo(scaledDig,CV_64F);
            rawData->imgAnalog->convertTo(scaledAna,CV_64F);

            combined = (scaledDig*(scaleFactor))+(scaledAna-8000);
            cv::convertScaleAbs(combined,adjMap,comDisplayScale);
            cv::imshow("combined",adjMap);

            rawData->imgGain->convertTo(scaledGain,CV_64F);
            cv::convertScaleAbs(scaledGain,adjMap,200);
            cv::imshow("gain",adjMap);

        }
        else if((ui->checkBox->isChecked()) && !(ui->checkBox_calib->isChecked()))
        {
            rawData->imgAnalog->convertTo(scaledAna,CV_64F);
            scaledAna -= dark;
            cv::convertScaleAbs(scaledAna, adjMap, anaDisplayScale);
            cv::imshow("analog", adjMap);

            cv::convertScaleAbs(*(rawData->imgDigital), adjMap, digDisplayScale);
            cv::imshow("digital", adjMap);

            scaleFactor = ui->lineEdit_3->text().toInt();
            rawData->imgDigital->convertTo(scaledDig,CV_64F);
            //rawData->imgAnalog->convertTo(scaledAna,CV_64F);

            combined = (scaledDig*(scaleFactor))+(scaledAna);
            cv::convertScaleAbs(combined,adjMap,comDisplayScale);
            cv::imshow("combined",adjMap);

            rawData->imgGain->convertTo(scaledGain,CV_64F);
            cv::convertScaleAbs(scaledGain,adjMap,200);
            cv::imshow("gain",adjMap);
        }
        else
        {
            cI= frame%2;

            if(ui->checkBox_evenOdd->isChecked())
            {
                cI = (frame+1)%2;
            }

            rawData->imgAnalog->convertTo(scaledAna,CV_64F);
            scaledAna -= darks[cI];
            cv::convertScaleAbs(scaledAna, adjMap, anaDisplayScale);
            cv::imshow("analog", adjMap);

            cv::convertScaleAbs(*(rawData->imgDigital), adjMap, digDisplayScale);
            cv::imshow("digital", adjMap);

            scaleFactor = ui->lineEdit_3->text().toInt();
            rawData->imgDigital->convertTo(scaledDig,CV_64F);
            //rawData->imgAnalog->convertTo(scaledAna,CV_64F);

            rawData->imgGain->convertTo(scaledGain,CV_64F);
            cv::threshold(scaledGain,notgainMask,0.5,1,1);
            cv::threshold(scaledGain,gainMask,0.5,1,0);

            combined = (gainMask.mul((scaledDig.mul(dADUdN[cI])+scaledAna-lgOffset[cI]))).mul(sRatio[cI])+ notgainMask.mul(scaledAna); //think this is correct
           // combined = (gainMask.mul((scaledDig.mul(dADUdN[cI])+scaledAna-lgOffset[cI]))).mul(sRatio[cI]);

            cv::convertScaleAbs(combined,adjMap,comDisplayScale);
            cv::imshow("combined",adjMap);

            //rawData->imgGain->convertTo(scaledGain,CV_64F);
            cv::convertScaleAbs(scaledGain,adjMap,200);
            cv::imshow("gain",adjMap);
        }
    }
    else
    {
        if(!(ui->checkBox->isChecked()))
        {

            rawData->imgAnalog->convertTo(scaledAna,CV_64F);
            scaledAna -= 8000;

            cv::log(scaledAna,scaledAna);
            cv::convertScaleAbs(scaledAna, adjMap, anaDisplayScale);
            cv::imshow("analog", adjMap);


            rawData->imgDigital->convertTo(scaledDig,CV_64F);
            scaledDig += 0.00001;
            cv::log(scaledDig,scaledDig);
            cv::convertScaleAbs(scaledDig, adjMap, digDisplayScale);
            cv::imshow("digital", adjMap);

            scaleFactor = ui->lineEdit_3->text().toInt();
            rawData->imgDigital->convertTo(scaledDig,CV_64F);
            rawData->imgAnalog->convertTo(scaledAna,CV_64F);

            combined = (scaledDig*(scaleFactor))+(scaledAna-8000);
            cv::log(combined,combined);
            cv::convertScaleAbs(combined,adjMap,comDisplayScale);
            cv::imshow("combined",adjMap);

            rawData->imgGain->convertTo(scaledGain,CV_64F);
            cv::convertScaleAbs(scaledGain,adjMap,200);
            cv::imshow("gain",adjMap);
        }
        else
        {
            rawData->imgAnalog->convertTo(scaledAna,CV_64F);
            scaledAna -= dark;
            cv::log(scaledAna,scaledAna);
            cv::convertScaleAbs(scaledAna, adjMap, anaDisplayScale);
            cv::imshow("analog", adjMap);

            rawData->imgDigital->convertTo(scaledDig,CV_64F);
            scaledDig += 0.00001;
            cv::log(scaledDig,scaledDig);
            cv::convertScaleAbs(scaledDig, adjMap, digDisplayScale);
            cv::imshow("digital", adjMap);

            scaleFactor = ui->lineEdit_3->text().toInt();
            rawData->imgDigital->convertTo(scaledDig,CV_64F);
            //rawData->imgAnalog->convertTo(scaledAna,CV_64F);

            combined = (scaledDig*(scaleFactor))+(scaledAna);
            cv::log(combined,combined);
            cv::convertScaleAbs(combined,adjMap,comDisplayScale);
            cv::imshow("combined",adjMap);

            rawData->imgGain->convertTo(scaledGain,CV_64F);
            cv::convertScaleAbs(scaledGain,adjMap,200);
            cv::imshow("gain",adjMap);
        }
    }

    cv::convertScaleAbs(results,adjMap,ui->lineEdit_resultsScale->text().toDouble());
    cv::imshow("results",adjMap);

    except = cv::waitKey(1);


}

void MainWindow::updatePixelValue()
{
    ui->label_13->setNum(mousex);
    ui->label_15->setNum(mousey);

    ui->label_anaPix->setNum(scaledAna.at<double>(mousey,mousex));
    ui->label_digPix->setNum(scaledDig.at<double>(mousey,mousex));
    ui->label_comPix->setNum(combined.at<double>(mousey,mousex));
    ui->label_gainPix->setNum(scaledGain.at<double>(mousey,mousex));
    ui->label_resPix->setNum(results.at<double>(mousey,mousex));
}

double MainWindow::getScaleFactor()
{
    return ui->lineEdit_3->text().toDouble();
}

void MainWindow::on_frameSlider_valueChanged(int value)
{
    rawData->goToFrame(uint(value));
    ui->label_3->setNum(int(rawData->getFrameNum()));
    updateDisplay();
    updatePixelValue();
}

void MainWindow::on_lineEdit_4_textEdited(const QString &arg1)
{
    anaDisplayScale = arg1.toDouble();
    updateDisplay();
}

void MainWindow::on_lineEdit_5_textEdited(const QString &arg1)
{
    digDisplayScale = arg1.toDouble();
    updateDisplay();
}

void MainWindow::on_lineEdit_6_textEdited(const QString &arg1)
{
    comDisplayScale = arg1.toDouble();
    updateDisplay();
}

void MainWindow::on_pushButton_4_clicked()
{
    uint minField;
    uint maxField;
    uint fstep;
    uint frms=0;

    fstep = ui->lineEdit_darkStep->text().toUInt();

    minField = ui->lineEdit_7->text().toUInt();
    maxField = ui->lineEdit_8->text().toUInt();

    if(maxField>(rawBGData->getNumOfFrames()-1))
    {
        maxField = (rawBGData->getNumOfFrames()-1);
    }
    if(minField>(rawBGData->getNumOfFrames()-1))
    {
        minField = (rawBGData->getNumOfFrames()-1);
    }
    dark = cv::Mat(128,128,CV_64F,cv::Scalar(0));
    if(minField<=maxField)
    {
        for(auto i = minField; i <= maxField; i=i+fstep)
        {
            rawBGData->goToFrame(i);
            rawBGData->imgAnalog->convertTo(scaledAna,CV_64F);
            dark += scaledAna;
            frms++;
        }
        //dark /= (maxField-minField)+1;
        dark /= frms;
    }
}


void MainWindow::on_checkBox_stateChanged(int arg1)
{
    updateDisplay();
    updatePixelValue();
}

void MainWindow::on_actionHistogram_Pixel_triggered()
{
    histui->show();
}

void MainWindow::on_actionPlot_Pixel_Values_triggered()
{
    pltPix->show();
}

void MainWindow::on_actionHistogram_Area_triggered()
{
    histAreaui->show();
}

void MainWindow::on_actionSave_Video_triggered()
{
    movDialog->show();
}

void MainWindow::on_actionCalc_Debounce_Offsets_triggered()
{
    calcOffsets->show();
}

void MainWindow::on_actionAdd_Frames_triggered()
{
    sumFrames->show();
}

void MainWindow::on_lineEdit_resultsScale_textEdited(const QString &arg1)
{
    updateDisplay();
}

uint MainWindow::getCalibEvenOdd()
{
    if(ui->checkBox_evenOdd->isChecked())
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void MainWindow::on_actionCalculate_Calibration_Darks_triggered()
{
    uint minField;
    uint maxField;
    uint fstep;
    uint frms=0;

    fstep = 2;

    minField = ui->lineEdit_7->text().toUInt();
    maxField = ui->lineEdit_8->text().toUInt();

    if(maxField>(rawBGData->getNumOfFrames()-1))
    {
        maxField = (rawBGData->getNumOfFrames()-1);
    }
    if(minField>(rawBGData->getNumOfFrames()-1))
    {
        minField = (rawBGData->getNumOfFrames()-1);
    }
    darks[0] = cv::Mat(128,128,CV_64F,cv::Scalar(0));
    darks[1] = cv::Mat(128,128,CV_64F,cv::Scalar(0));

    if(minField<=maxField)
    {
        for(auto i = minField; i <= maxField; i=i+fstep)
        {
            rawBGData->goToFrame(i);
            rawBGData->imgAnalog->convertTo(scaledAna,CV_64F);
            darks[i%2] += scaledAna;
            frms++;
        }
        //dark /= (maxField-minField)+1;
        darks[minField%2] /= frms;
    }

    minField++;
    frms = 0;

    if(minField<=maxField)
    {
        for(auto i = minField; i <= maxField; i=i+fstep)
        {
            rawBGData->goToFrame(i);
            rawBGData->imgAnalog->convertTo(scaledAna,CV_64F);
            darks[i%2] += scaledAna;
            frms++;
        }
        //dark /= (maxField-minField)+1;
        darks[minField%2] /= frms;
    }


}

void MainWindow::on_checkBox_calib_stateChanged(int arg1)
{
    updateDisplay();
    updatePixelValue();
}

void MainWindow::on_actionExport_Calibrated_Data_triggered()
{
    uint maxindex   = rawData->getNumOfFrames();
    uint preFrame = rawData->getFrameNum();
    QString filename;
    uint cI;
    uint frame;
    filename        = QFileDialog::getSaveFileName(this, tr("Save data to:"), "f:/", tr("*.craw"));
    boost::iostreams::mapped_file_params params;
    params.path     = filename.toStdString();
    params.new_file_size =  sizeof(uint32_t)*128*128*maxindex;
    params.flags         = boost::iostreams::mapped_file::mapmode::readwrite;
    cv::Mat *output;
    output = nullptr;
    boost::iostreams::mapped_file_sink out(params);
    QProgressDialog progress("Exporting data to file...","cancel",0, int(maxindex));

    progress.setWindowModality(Qt::WindowModal);

    for(unsigned long i = 0; i<maxindex; i++)
    {
        if(output!=nullptr)
        {
            delete output;
        }

        output = new cv::Mat(int(128),int(128),CV_32FC1,reinterpret_cast<uchar *>(const_cast<char *>(out.data()))+(i*128*128*sizeof(uint32_t)));

        rawData->goToFrame(i);
        frame = rawData->getFrameNum();
        cI= frame%2;

        if(ui->checkBox_evenOdd->isChecked())
        {
            cI = (frame+1)%2;
        }

        rawData->imgAnalog->convertTo(scaledAna,CV_64F);
        scaledAna -= darks[cI];
        rawData->imgDigital->convertTo(scaledDig,CV_64F);       
        rawData->imgGain->convertTo(scaledGain,CV_64F);
        cv::threshold(scaledGain,notgainMask,0.5,1,1);
        cv::threshold(scaledGain,gainMask,0.5,1,0);
        combined = (gainMask.mul((scaledDig.mul(dADUdN[cI])+scaledAna-lgOffset[cI]))).mul(sRatio[cI])+ notgainMask.mul(scaledAna); //think this is correct
        combined.convertTo(*output,CV_32FC1);
        progress.setValue(int(i));
    }

    if(out.is_open())
    {
        out.close();
    }
    rawData->goToFrame(preFrame);
}

void MainWindow::on_checkBox_evenOdd_stateChanged(int arg1)
{
    updateDisplay();
    updatePixelValue();
}

void MainWindow::on_actionExport_Calibrated_Data_with_offsets_triggered()
{
    uint maxindex   = rawData->getNumOfFrames();

    if(offsetsCalib.size() == maxindex)
    {
        ui->label_20->setText(QString(""));
        uint preFrame = rawData->getFrameNum();
        QString filename;
        uint cI;
        uint frame;
        filename        = QFileDialog::getSaveFileName(this, tr("Save data to:"), "f:/", tr("*.craw"));
        boost::iostreams::mapped_file_params params;
        params.path     = filename.toStdString();
        params.new_file_size =  sizeof(uint32_t)*128*128*maxindex;
        params.flags         = boost::iostreams::mapped_file::mapmode::readwrite;
        cv::Mat *output;
        output = nullptr;
        boost::iostreams::mapped_file_sink out(params);
        QProgressDialog progress("Exporting data to file...","cancel",0, int(maxindex));

        progress.setWindowModality(Qt::WindowModal);

        for(unsigned long i = 0; i<maxindex; i++)
        {
            if(output!=nullptr)
            {
                delete output;
            }

            output = new cv::Mat(int(128),int(128),CV_32FC1,reinterpret_cast<uchar *>(const_cast<char *>(out.data()))+(i*128*128*sizeof(uint32_t)));

            rawData->goToFrame(i);
            frame = rawData->getFrameNum();
            cI= frame%2;

            if(ui->checkBox_evenOdd->isChecked())
            {
                cI = (frame+1)%2;
            }

            rawData->imgAnalog->convertTo(scaledAna,CV_64F);
            scaledAna -= darks[cI];
            rawData->imgDigital->convertTo(scaledDig,CV_64F);
            rawData->imgGain->convertTo(scaledGain,CV_64F);
            cv::threshold(scaledGain,notgainMask,0.5,1,1);
            cv::threshold(scaledGain,gainMask,0.5,1,0);
            combined = (gainMask.mul((scaledDig.mul(dADUdN[cI])+scaledAna-lgOffset[cI]))).mul(sRatio[cI])+ notgainMask.mul(scaledAna); //think this is correct
            combined -= offsetsCalib[i];
            combined.convertTo(*output,CV_32F);
            progress.setValue(int(i));
        }

        if(out.is_open())
        {
            out.close();
        }
        rawData->goToFrame(preFrame);
    }
    else
    {
        ui->label_20->setText(QString("Must calculate offsets for calibrated data..."));
    }
}

void MainWindow::on_actionEven_Calib_Darks_to_Results_triggered()
{
    results = darks[0];
    updateDisplay();
    updatePixelValue();
}

void MainWindow::on_actionDarks_to_Results_triggered()
{
    results = dark;
    updateDisplay();
    updatePixelValue();
}

void MainWindow::on_actionOdd_Calib_Dakrs_to_Results_triggered()
{
    results = darks[1];
    updateDisplay();
    updatePixelValue();
}

void MainWindow::on_actionCopy_single_darks_to_calibration_darks_triggered()
{
    darks[0]=dark;
    darks[1]=dark;
    updateDisplay();
    updatePixelValue();
}
