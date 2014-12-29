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
        
        Delaunay( Vec2i size = Vec2i( 320, 240 ) ){
            
            // SIFTまたはSURFを使う場合はこれを呼び出す．
            //::cv::initModule_nonfree();
            this->_detector = ::cv::FeatureDetector::create("FAST");
            
            init( size );
            
        };
        ~Delaunay(){
            
        };
        
        void init( Vec2i size = Vec2i( 320, 240 ) ){
            _size = size;
            _subDiv2D.initDelaunay( ::cv::Rect(0,0,_size.x,_size.y) );
        }
        
        void insertPoint( ::cv::Point2f point ){
            _subDiv2D.insert( point );
        }
        void insertPoint( vector<::cv::Point2f> points ){
            _subDiv2D.insert( points );
        }
        
        void insertPoint( ci::gl::Texture texture ){
            
            _img = ::cv::Mat( toOcv( texture, CV_8UC3 ) );
            
            // 特徴点抽出の実行
            vector<::cv::KeyPoint> keypoint;
            _detector->detect( _img, keypoint );
            
            vector<::cv::Point2f> points;
            vector<::cv::KeyPoint>::iterator it = keypoint.begin(), it_end = keypoint.end();
            for(; it!=it_end; ++it) {
                points.push_back( it->pt );
            }
            
            init( texture.getSize() );
            this->insertPoint( points );
            
        }
        
        void update(){
            _subDiv2D.getTriangleList( _triangles );
        };
        
        void draw(){
            _draw( 1.0, 1.0 );
        }
        
        void draw( Vec2i size ){
            _draw( (float)size.x / (float)_size.x, (float)size.y / (float)_size.y );
        }
        
    private:
        
        void _draw( float scaleX = 1.0, float scaleY = 1.0 ){
            
            glBegin(GL_TRIANGLES);
            for(auto it = _triangles.begin(), end=_triangles.end(); it != end; ++it )
            {
                
                ::cv::Vec6f &vec = *it;
                
                Vec2f p1( vec[0] * scaleX, vec[1] * scaleY );
                Vec2f p2( vec[2] * scaleX, vec[3] * scaleY );
                Vec2f p3( vec[4] * scaleX, vec[5] * scaleY );
                
                if( 100 < (p1-p2).length() || 100 < (p2-p3).length() || 100 < (p3-p1).length() ){
                    continue;
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
                ci::gl::vertex( p1 );
                ci::gl::vertex( p2 );
                ci::gl::vertex( p3 );
                
            }
            glEnd();
            
        }
        
        
        pthread_mutex_t _mutex;
        std::thread mThread;
        
        ::cv::Mat _img;
        ::cv::Ptr<::cv::FeatureDetector> _detector;
        
        Vec2i               _size;
        ::cv::Subdiv2D      _subDiv2D;
        vector<::cv::Vec6f> _triangles;
        
    };
    
}}