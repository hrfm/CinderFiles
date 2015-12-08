#include "Delaunay.h"

using namespace std;
using namespace ci;

namespace hrfm{ namespace cv{
    
    //! public:

    Delaunay::Delaunay( ivec2 size ){
        
        // SIFTまたはSURFを使う場合はこれを呼び出す．
        //::cv::initModule_nonfree();
        //this->_detector = ::cv::FeatureDetector::create("FAST");
        
        init( size );
        
    };
    Delaunay::~Delaunay(){};
    
    void Delaunay::init( ivec2 size ){
        _size = size;
        _subDiv2D.initDelaunay( ::cv::Rect(0,0,_size.x,_size.y) );
    }
    
    void Delaunay::insertPoint( ::cv::Point2f point ){
        _subDiv2D.insert( point );
    }
    void Delaunay::insertPoint( vector<::cv::Point2f> points ){
        _subDiv2D.insert( points );
    }
    
    void Delaunay::insertPoint( ci::gl::TextureRef texture ){
        
        // _img = ::cv::Mat( toOcv( texture, CV_8UC3 ) );
        // !!!!!!
        
        // 特徴点抽出の実行
        vector<::cv::KeyPoint> keypoint;
        _detector->detect( _img, keypoint );
        
        vector<::cv::Point2f> points;
        vector<::cv::KeyPoint>::iterator it = keypoint.begin(), it_end = keypoint.end();
        for(; it!=it_end; ++it) {
            points.push_back( it->pt );
        }
        
        init( texture->getSize() );
        this->insertPoint( points );
        
    }
    
    void Delaunay::update(){
        _subDiv2D.getTriangleList( _triangles );
    };
    
    void Delaunay::draw( float threshold, GLint type ){
        _draw( 1.0, 1.0, threshold, type );
    }
    
    void Delaunay::draw( ivec2 size, float threshold, GLint type ){
        _draw( (float)size.x / (float)_size.x, (float)size.y / (float)_size.y, threshold, type );
    }
    
    //! private:
    
    void Delaunay::_draw( float scaleX, float scaleY, float threshold, GLint type ){
        
        ci::gl::begin(type);
        
        for( auto it = _triangles.begin(), end=_triangles.end(); it != end; ++it )
        {
            
            ::cv::Vec6f &vec = *it;
            
            vec2 p1( vec[0] * scaleX, vec[1] * scaleY );
            vec2 p2( vec[2] * scaleX, vec[3] * scaleY );
            vec2 p3( vec[4] * scaleX, vec[5] * scaleY );
            
            if( 0 < threshold ){
                if( threshold < ci::length(p1-p2) || threshold < ci::length(p2-p3) || threshold < ci::length(p3-p1) ){
                    continue;
                }
            }
            
            int gX = floor( (vec[0]+vec[2]+vec[4]) / 3.0 );
            if( gX < 0 ){
                gX = 0;
            }else if( _size.x <= gX ){
                gX = _size.x - 1;
            }
            
            int gY = floor( (vec[1]+vec[3]+vec[5]) / 3.0 );
            if( gY < 0 ){
                gY = 0;
            }else if( _size.y <= gY ){
                gY = _size.y - 1;
            }
            
            ::cv::Vec3b color = _img.at<::cv::Vec3b>(gY,gX);
            
            int B = color[0];
            int G = color[1];
            int R = color[2];
            
            ci::gl::color( ci::Color( R / 255.0, G / 255.0, B / 255.0 ) );
            if( type == GL_LINES ){
                ci::gl::vertex( p1 );
                ci::gl::vertex( p2 );
                ci::gl::vertex( p2 );
                ci::gl::vertex( p3 );
                ci::gl::vertex( p3 );
                ci::gl::vertex( p1 );
            }else{
                ci::gl::vertex( p1 );
                ci::gl::vertex( p2 );
                ci::gl::vertex( p3 );
            }
            
        }
        
        ci::gl::end();
        
    }
    
}}