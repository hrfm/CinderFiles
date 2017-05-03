#pragma once

#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/Rand.h"
#include "CinderOpenCV.h"
//#include "opencv2/nonfree/nonfree.hpp"

using namespace std;
using namespace ci;
using namespace ci::app;

namespace hrfm{ namespace cv{
    
    class DftIdft{
        
    public:
        
        DftIdft( string file );
        ~DftIdft();
        
        void init( string file );
        void update();
        void draw( float size = 128.0 );
        void pick( int mouseX, int mouseY, int step = 1 );
        void clear();
        
    private:
        
        float _size = 128.0;
        
        ::cv::Mat src_img, mag_img;
        ::cv::Mat Re_img, Im_img, Complex_img;
        ::cv::Mat zero, dft_src, dft_dst, dft_dst_p;
        ::cv::Mat idft_img;
        vector<::cv::Mat> mv;
        int src_cols, src_rows, dft_cols, dft_rows;
        
        void _calcMagImage();
        void _shiftDFT( ::cv::Mat&src, ::cv::Mat&dst );
        void _calcIDFT(bool all = false);
        
    };
    
}}