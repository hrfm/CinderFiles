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
using namespace ci::app;

namespace hrfm{ namespace cv{
    
    class OpticalFlow{
        
    public:
        
        OpticalFlow( Vec2i textureSize ){
            
            // --- Create FBO. ---
            
            ci::gl::Fbo::Format captureFormat;
            mOpticalFlowFBO    = ci::gl::Fbo( textureSize.x, textureSize.y, captureFormat );
            mOpticalFlowBounds = mOpticalFlowFBO.getBounds();
            
            // --- Create ForceMap. ---
            
            mForceMap.setup( 16 * 4, 9 * 4 );
            
        };
        
        ~OpticalFlow(){};
        
        void update( ci::gl::Texture texture, float bias = 1.0, float frameRate = 30.0 );
        
        void draw();
        
        ForceMap getForceMap();
        
        ci::gl::Texture getOpticalFlowTexture();
        
        Vec2i getOpticalFlowTextureSize();
        
        Rectf getOpticalFlowTextureBounds();
        
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