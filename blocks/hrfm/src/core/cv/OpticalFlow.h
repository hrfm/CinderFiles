#pragma once

#include "cinder/app/AppBasic.h"
#include "cinder/gl/Fbo.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/GlslProg.h"
#include "CinderOpenCV.h"
#include "XmlLoader.h"

#include "ForceMap.h"

using namespace std;
using namespace ci;

namespace hrfm{ namespace cv{
    
    class OpticalFlow{
        
    public:
        
        OpticalFlow( Vec2i textureSize, int forceMapScale = 4 );
        ~OpticalFlow(){};
        
        void update( ci::gl::Texture texture, float bias = 1.0, float frameRate = 30.0 );
        
        void draw();
        void draw( Rectf bounds );
        
        ForceMap getForceMap();
        ForceMap * getForceMapPtr();
        
        ci::gl::Texture getTexture();
        Vec2i getSize();
        Rectf getBounds();
        
        vector<::cv::Point2f> prevFeatures, features;
        vector<uint8_t>		  featureStatuses;
        
    private:
        
        //! private
        
        void _chooseFeatures( ::cv::Mat currentFrame );
        void _trackFeatures( ::cv::Mat currentFrame );
        
        float recentTime;
        
        static const int MAX_FEATURES = 256;
        
        ci::gl::Fbo   mOpticalFlowFBO;
        Rectf     mOpticalFlowBounds;
        ::cv::Mat mPrevFrame;
        ForceMap  mForceMap;
        
    };
    
}}