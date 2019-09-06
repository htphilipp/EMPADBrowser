#include "empad2lib.h"


Empad2::Empad2(uint xdiml, uint ydiml, std::string rawname)
{
    xdim = xdiml;
    ydim = ydiml;
    uint16_t pixtemp;
    uint16_t digtemp;
    mem_ifile.open(std::string(rawname));
    frames=uint(mem_ifile.size()/(xdim*ydim*sizeof (uint32_t)));
   // Raw = new std::vector<uint16_t>(reinterpret_cast<uchar *>(const_cast<char *>(mem_ifile.data()))+(0*2*xdim*ydim*sizeof(uint16_t)),reinterpret_cast<uchar *>(const_cast<char *>(mem_ifile.data()))+(1*2*xdim*ydim*sizeof(uint16_t)-1));
    Raw = new cv::Mat(int(ydim),int(2*xdim),CV_16UC1,reinterpret_cast<uchar *>(const_cast<char *>(mem_ifile.data()))+(0*2*xdim*ydim*sizeof (uint16_t)));
    imgAnalog = new cv::Mat(int(ydim),int(xdim),CV_16UC1);
    imgDigital = new cv::Mat(int(ydim),int(xdim),CV_16UC1);
    imgGain = new cv::Mat(int(ydim),int(xdim),CV_16UC1);

    for(int i = 0; i<int(xdim); i ++)
    {   for( int j = 0; j<int(ydim); j++)
        {
            pixtemp = Raw->at<uint16_t>(i,j*2);
            //std::cout<<pixtemp<<std::endl;
            imgAnalog->at<uint16_t>(i,j) = 0;
            imgAnalog->at<uint16_t>(i,j) =  (pixtemp&(0x3FFF));
            imgDigital->at<uint16_t>(i,j) = 0;
            digtemp = (pixtemp>>14);

            pixtemp = Raw->at<uint16_t>(i,j*2+1);
            digtemp = (pixtemp<<2)+digtemp;
            imgDigital->at<uint16_t>(i,j) = digtemp;

            imgDigital->at<uint16_t>(i,j) = ((pixtemp<<2)&(0xFFFC))&(digtemp);
            imgGain->at<uint16_t>(i,j) =    (pixtemp&(0x8000))>>15;
        }
    }
}

uint Empad2::goToFrame(uint frameNum)
{
    uint16_t pixtemp;
    uint16_t digtemp;
    if(frameNum<frames)
    {
        presentFrame = frameNum;

    }
    else
    {
        presentFrame = frames-1;
    }
    delete Raw;
    //Raw = new std::vector<uint16_t>(reinterpret_cast<uchar *>(const_cast<char *>(mem_ifile.data()))+(presentFrame*2*xdim*ydim*sizeof(uint16_t)),reinterpret_cast<uchar *>(const_cast<char *>(mem_ifile.data()))+((presentFrame+1)*2*xdim*ydim*sizeof(uint16_t)-1));
    Raw = new cv::Mat(int(ydim),int(2*xdim),CV_16UC1,reinterpret_cast<uchar *>(const_cast<char *>(mem_ifile.data()))+(presentFrame*2*xdim*ydim*sizeof (unsigned short)));

    for(int i = 0; i<int(xdim); i ++)
    {   for( int j = 0; j<int(ydim); j++)
        {
            pixtemp = Raw->at<uint16_t>(i,j*2);
            imgAnalog->at<uint16_t>(i,j) =  (pixtemp&0x3FFF);
            imgDigital->at<uint16_t>(i,j) = 0;
            //imgDigital->at<uint16_t>(i,j) = (pixtemp&(0xC000))>>14;
            digtemp = (pixtemp>>14);

            pixtemp = Raw->at<uint16_t>(i,j*2+1);
            digtemp = (pixtemp<<2)+digtemp;
            imgDigital->at<uint16_t>(i,j) = digtemp;
            imgGain->at<uint16_t>(i,j) =    (pixtemp&0x8000)>>15;
        }
    }

//    Raw = new std::vector<uint32_t>(reinterpret_cast<uchar *>(const_cast<char *>(mem_ifile.data()))+(presentFrame*xdim*ydim*sizeof(uint32_t)),reinterpret_cast<uchar *>(const_cast<char *>(mem_ifile.data()))+(presentFrame*xdim*ydim*sizeof(uint32_t))-1);
//    for(uint i = 0; i<xdim; i ++)
//    {   for( uint j = 0; j<ydim; j++)
//        {
//            pixtemp = Raw->at(i+j*xdim);
//            imgAnalog->at<uint16_t>(i,j) =  uint16_t((Raw->at(i+j*xdim)&0b00000000000000000011111111111111));
//            imgDigital->at<uint16_t>(i,j) = uint16_t((Raw->at(i+j*xdim)&0b00111111111111111100000000000000)>>14);
//            imgGain->at<uint16_t>(i,j) =    uint16_t((Raw->at(i+j*xdim)&0b10000000000000000000000000000000)>>31);
//        }
//    }
    return(presentFrame);
}

uint Empad2::getFrameNum()
{
    return(presentFrame);
}

uint Empad2::getNumOfFrames()
{
    return(frames);
}

uint Empad2::getxdim()
{
    return(xdim);
}

uint Empad2::getydim()
{
    return(ydim);
}

Empad2::~Empad2()
{
    delete Raw;

    if(mem_ifile.is_open())
    {
        mem_ifile.close();
    }
}

uint Empad2::getAnalog(uint x, uint y, uint frm)
{
    uint pixtemp;
    if(frm>=frames)
    {
        frm = frames-1;
    }
    cv::Mat *tempMat = new cv::Mat(int(ydim),int(2*xdim),CV_16UC1,reinterpret_cast<uchar *>(const_cast<char *>(mem_ifile.data()))+(frm*2*xdim*ydim*sizeof (unsigned short)));;

    if((x<xdim) && (y<ydim))
    {
        pixtemp = tempMat->at<uint16_t>(int(x),int(y)*2);
    }
    else
    {
        pixtemp = 0;
    }

    delete tempMat;
    return (pixtemp&0x3FFF);
}

uint Empad2::getDigital(uint x, uint y, uint frm)
{
    uint pixtemp;
    uint digtemp;
    if(frm>=frames)
    {
        frm = frames-1;
    }
    cv::Mat *tempMat = new cv::Mat(int(ydim),int(2*xdim),CV_16UC1,reinterpret_cast<uchar *>(const_cast<char *>(mem_ifile.data()))+(frm*2*xdim*ydim*sizeof (unsigned short)));;

    if((x<xdim) && (y<ydim))
    {
        pixtemp = tempMat->at<uint16_t>(int(x),int(y)*2);
        digtemp = (pixtemp>>14);
        pixtemp = tempMat->at<uint16_t>(int(x),int(y)*2+1);
        digtemp = (pixtemp<<2)+digtemp;
    }
    else
    {
        pixtemp = 0;
        digtemp = 0;
    }

    delete tempMat;
    return digtemp;
}

uint Empad2::getGain(uint x, uint y, uint frm)
{
    uint pixtemp;
    if(frm>=frames)
    {
        frm = frames-1;
    }
    cv::Mat *tempMat = new cv::Mat(int(ydim),int(2*xdim),CV_16UC1,reinterpret_cast<uchar *>(const_cast<char *>(mem_ifile.data()))+(frm*2*xdim*ydim*sizeof (unsigned short)));;

    if((x<xdim) && (y<ydim))
    {
        pixtemp = tempMat->at<uint16_t>(int(x),int(y)*2+1);

    }
    else
    {
        pixtemp = 0;
    }

    delete tempMat;
    return ((pixtemp&0x8000)>>15);
}
