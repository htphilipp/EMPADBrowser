#ifndef EMPAD2LIB_H
#define EMPAD2LIB_H
#include<opencv2/opencv.hpp>
#include<opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

#include <boost/iostreams/device/mapped_file.hpp>
#include <string>
#include <vector>
//comment: Hugh Philipp Wrote this
class Empad2
{

public:
    Empad2();

    Empad2(uint, uint, std::string);
    uint goToFrame(uint);
    cv::Mat *imgAnalog;
    cv::Mat *imgDigital;
    cv::Mat *imgGain;
    //std::vector<uint16_t> *Raw;
    cv::Mat *Raw;
    uint getydim();
    uint getxdim();
    uint getFrameNum();
    uint getNumOfFrames();
    ~Empad2();
    uint getAnalog(uint x, uint y, uint frm);
    uint getDigital(uint x, uint y, uint frm);
    uint getGain(uint x,uint y, uint frm);
   // std::vector<double> offsets;

private:
    boost::iostreams::mapped_file_source mem_ifile;
    uint presentFrame;
    uint frames;
    uint xdim,ydim;


};

#endif // EMPAD2LIB_H
