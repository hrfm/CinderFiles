#include "DftIdft.h"

using namespace std;
using namespace ci;
using namespace ci::app;

namespace hrfm{ namespace cv{
    
    //! public:
    
    DftIdft::DftIdft( string file ){
        init( file );
    };
    DftIdft::~DftIdft(){};
    
    void DftIdft::init( string file ){
        
        // --- DFT
        
        src_img = ::cv::Mat( toOcv( ci::gl::Texture( loadImage( DataLoader::load(file) ) ), CV_8UC1 ) );
        
        ::cv::Size s_size = src_img.size();
        src_cols          = s_size.width;
        src_rows          = s_size.height;
        
        Re_img      = ::cv::Mat(s_size, CV_64F);
        Im_img      = ::cv::Mat::zeros(s_size, CV_64F);
        Complex_img = ::cv::Mat(s_size, CV_64FC2);
        
        src_img.convertTo(Re_img, CV_64F);
        mv.push_back(Re_img);
        mv.push_back(Im_img);
        merge(mv, Complex_img);
        
        idft_img = ::cv::Mat::zeros(s_size, CV_64F);
        zero     = ::cv::Mat::zeros(s_size, CV_64F);
        
        _calcMagImage();
        
    }
    
    void DftIdft::update(){
        
        _calcIDFT();
        
    };
    
    void DftIdft::draw( float size ){
        
        _size = size;
        
        ::cv::Mat mag_mt, idft_mt;
        mag_img.convertTo(mag_mt, CV_32F);
        idft_img.convertTo(idft_mt, CV_32F);
        
        ci::gl::draw( ci::gl::Texture( fromOcv(src_img) ), ci::Rectf(   0.0, 0.0, _size, _size ) );
        ci::gl::draw( ci::gl::Texture( fromOcv(mag_mt)  ), ci::Rectf( _size, 0.0, _size*2.0, _size ) );
        ci::gl::draw( ci::gl::Texture( fromOcv(idft_mt) ), ci::Rectf( _size*2.0, 0.0, _size*3.0, _size ) );
        
    }
    
    void DftIdft::clear(){
        _calcMagImage();
    }
    
    void DftIdft::pick( int mouseX, int mouseY, int step ){
        
        step = 10;
        
        float dx = 0.0, dy = 0.0;
        
        if( _size <= mouseX && mouseX < _size*2 ){
            if( mouseY < _size ){
                dx = (float)(mouseX-_size)/_size;
                dy = (float)mouseY/_size;
            }
        }
        
        int cx = src_cols/2;
        int cy = src_rows/2;
        int w  = dft_cols;
        int h  = dft_rows;
        
        int x  = w * dx;
        int y  = h * dy;
        int mx = x;
        int my = y;
        
        for(int j=-step; j<=step; j++) {
            
            my = y+j + ((y+j)<cy ? cy:-cy);
            
            if( y+j<0 || y+j>=h ){
                continue;
            }
            
            double *from = dft_dst.ptr<double>(my);
            double *to = dft_dst_p.ptr<double>(my);
            double *mag = mag_img.ptr<double>(y+j);
            
            for(int i=-step; i<=step; i++) {
                mx = x+i + ((x+i)<cx ? cx:-cx);
                if(x+i<0 || x+i>=w) break;
                to[(mx)*2+0] = from[(mx)*2+0];
                to[(mx)*2+1] = from[(mx)*2+1];
                mag[x+i] = 0;
            }
            
        }
        
    }
    
    //! private:
    
    // calc magnitude image
    void DftIdft::_calcMagImage(){
        
        dft_rows = ::cv::getOptimalDFTSize(src_rows);
        dft_cols = ::cv::getOptimalDFTSize(src_cols);
        dft_src  = ::cv::Mat::zeros(dft_rows, dft_cols, CV_64FC2);
        
        ::cv::Mat roi(dft_src, ::cv::Rect(0, 0, src_cols, src_rows));
        Complex_img.copyTo(roi);
        
        ::cv::dft(dft_src, dft_dst);
        dft_dst_p = ::cv::Mat::zeros(dft_dst.size(), dft_dst.type());
        
        //split(dft_dst.mul(dft_dst), mv);
        //sqrt(mv[0]+mv[1], mv[0]);
        ::cv::split(dft_dst, mv);
        ::cv::magnitude(mv[0], mv[1], mv[0]);
        ::cv::log(mv[0]+1, mv[0]); // for ver. 2.1 or later
        
        _shiftDFT(mv[0], mv[0]);
        
        //double min, max;
        //minMaxLoc(mv[0], &min, &max);
        //mag_img = mv[0]*1.0/(max-min) - 1.0*min/(max-min);
        ::cv::normalize(mv[0], mag_img, 0, 1, CV_MINMAX);
        
    }
    
    // swap 1,3 and 2,4 quadrants respectively
    void DftIdft::_shiftDFT( ::cv::Mat &src, ::cv::Mat& dst)
    {
        ::cv::Mat tmp;
        int cx = src.cols/2;
        int cy = src.rows/2;
        
        for( int i=0; i<=cx; i+=cx) {
            ::cv::Mat qs(src, ::cv::Rect(i^cx,0,cx,cy));
            ::cv::Mat qd(dst, ::cv::Rect(i,cy,cx,cy));
            qs.copyTo(tmp);
            qd.copyTo(qs);
            tmp.copyTo(qd);
        }
        
    }
    
    // Inverse Discrete Fourier Transforma
    void DftIdft::_calcIDFT(bool all){
        
        if(all) {
            dft_dst.copyTo(dft_dst_p);
            mag_img.setTo(0);
        }
        
        double min, max;
        ::cv::idft( dft_dst_p, dft_src );
        ::cv::split( dft_src, mv );
        ::cv::minMaxLoc( mv[0], &min, &max );
        
        idft_img = ::cv::Mat( mv[0]*1.0/max, ::cv::Rect(0, 0, src_cols, src_rows));
        
    }
    
}}