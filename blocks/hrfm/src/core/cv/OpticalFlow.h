#pragma once

#include "cinder/app/App.h"
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
        
        OpticalFlow( ivec2 textureSize, int forceMapScale = 4 );
        ~OpticalFlow(){};
        
        void update( ci::gl::TextureRef texture, float bias = 1.0, float frameRate = 30.0 );
        
        void draw();
        void draw( Rectf bounds );
        
        ForceMap getForceMap();
        ForceMap * getForceMapPtr();
        
        ci::gl::TextureRef getTexture();
        ivec2 getSize();
        Rectf getBounds();
        
        vector<::cv::Point2f> prevFeatures, features;
        vector<uint8_t>		  featureStatuses;
        
    private:
        
        //! private
        
        void _chooseFeatures( ::cv::Mat currentFrame );
        void _trackFeatures( ::cv::Mat currentFrame );
        
        float recentTime;
        
        static const int MAX_FEATURES = 256;
        
        ci::gl::FboRef mOpticalFlowFBO;
        Rectf     mOpticalFlowBounds;
        ::cv::Mat mPrevFrame;
        ForceMap  mForceMap;
        
    };
    
}}