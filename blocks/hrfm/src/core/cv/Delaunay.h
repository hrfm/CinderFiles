#pragma once

#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/Rand.h"
#include "CinderOpenCV.h"
#include "opencv2/nonfree/nonfree.hpp"

using namespace std;
using namespace ci;
using namespace ci::app;

namespace hrfm{ namespace cv{
    
    class Delaunay{
        
    public:
        
        Delaunay( Vec2i size = Vec2i( 320, 240 ) );
        ~Delaunay();
        
        void init( Vec2i size = Vec2i( 320, 240 ) );
        
        void insertPoint( ::cv::Point2f point );
        void insertPoint( vector<::cv::Point2f> points );
        void insertPoint( ci::gl::Texture texture );
        
        void update();
        
        void draw( float threshold = 0.0, GLint type = GL_TRIANGLES );
        void draw( Vec2i size, float threshold = 0.0, GLint type = GL_TRIANGLES );
        
    private:
        
        void _draw( float scaleX = 1.0, float scaleY = 1.0, float threshold = 0.0, GLint type = GL_TRIANGLES );
        
        pthread_mutex_t _mutex;
        std::thread mThread;
        
        ::cv::Mat _img;
        ::cv::Ptr<::cv::FeatureDetector> _detector;
        
        Vec2i               _size;
        ::cv::Subdiv2D      _subDiv2D;
        vector<::cv::Vec6f> _triangles;
        
    };
    
}}