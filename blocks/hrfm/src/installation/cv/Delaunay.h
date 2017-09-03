#pragma once

#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/Rand.h"
#include "CinderOpenCV.h"

using namespace std;
using namespace ci;

namespace hrfm{ namespace cv{
    
    class Delaunay{
        
    public:
        
        Delaunay( ivec2 size = ivec2( 320, 240 ) );
        ~Delaunay();
        
        void init( ivec2 size = ivec2( 320, 240 ) );
        
        void insertPoint( ::cv::Point2f point );
        void insertPoint( vector<::cv::Point2f> points );
        void insertPoint( ci::gl::TextureRef texture );
        
        void update();
        
        void draw( float threshold = 0.0, GLint type = GL_TRIANGLES );
        void draw( ivec2 size, float threshold = 0.0, GLint type = GL_TRIANGLES );
        
    private:
        
        void _draw( float scaleX = 1.0, float scaleY = 1.0, float threshold = 0.0, GLint type = GL_TRIANGLES );
        
        pthread_mutex_t _mutex;
        std::thread mThread;
        
        ::cv::Mat _img;
        ::cv::Ptr<::cv::FeatureDetector> _detector;
        
        ivec2               _size;
        ::cv::Subdiv2D      _subDiv2D;
        vector<::cv::Vec6f> _triangles;
        
    };
    
}}