#include "OpticalFlow.h"

using namespace std;
using namespace ci;
using namespace ci::app;

namespace hrfm{ namespace cv{
    
    void OpticalFlow::update( gl::Texture texture, float bias, float frameRate ){
        
        mOpticalFlowFBO.bindFramebuffer();
        gl::pushMatrices();
        gl::setMatricesWindow( getWindowSize(), false );
        gl::clear();
        gl::color(1.0f,1.0f,1.0f);
        glPushMatrix();
        gl::draw( texture, mOpticalFlowBounds );
        glPopMatrix();
        gl::popMatrices();
        mOpticalFlowFBO.unbindFramebuffer();
        
        if( 1.0 / frameRate < getElapsedSeconds() - recentTime ){
            
            ::cv::Mat currentFrame( toOcv( Channel( Surface( mOpticalFlowFBO.getTexture() ) ) ) );
            
            if( mPrevFrame.data ) {
                if( features.empty() || getElapsedFrames() % 30 == 0 ){
                    // pick new features once every 30 frames, or the first frame
                    _chooseFeatures( mPrevFrame );
                }
                _trackFeatures( currentFrame );
            }
            
            mPrevFrame = currentFrame;
            
            // --- Update ForceMap. ---
            
            Vec2f from, to, size( mOpticalFlowFBO.getSize() );
            float dist;
            
            int max = features.size();
            for( size_t idx = 0; idx < max; ++idx ) {
                if( featureStatuses[idx] ) {
                    from = fromOcv( features[idx] );
                    to   = fromOcv( prevFeatures[idx] );
                    dist = from.distance(to);
                    if( 10.f < dist && dist < 50.0f ){
                        mForceMap.addForce( from / size, ( to - from ) * bias );
                    }
                }
            }
            
            recentTime = getElapsedSeconds();
            
        }
        
        mForceMap.update();
        
    }
    
    void OpticalFlow::draw(){
        
        Vec2f from, to;
        float dist;
        int length = features.size();
        
        gl::lineWidth(2.0);
        glDisable( GL_TEXTURE_2D );
        
        glColor4f( 1, 1, 0, 0.5f );
        // draw all the old points
        for( vector<::cv::Point2f>::const_iterator featureIt = prevFeatures.begin(); featureIt != prevFeatures.end(); ++featureIt ){
            gl::drawStrokedCircle( fromOcv( *featureIt ), 1 );
        }
        
        // draw all the new points
        for( vector<::cv::Point2f>::const_iterator featureIt = features.begin(); featureIt != features.end(); ++featureIt ){
            gl::drawSolidCircle( fromOcv( *featureIt ), 1 );
        }
        
        // draw the lines connecting them
        glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
        
        glBegin( GL_LINES );
        {
            for( size_t idx = 0; idx < length; ++idx ) {
                if( featureStatuses[idx] ) {
                    from = fromOcv( features[idx] );
                    to   = fromOcv( prevFeatures[idx] );
                    dist = from.distance(to);
                    if( dist < 40.0 ){
                        gl::vertex( from );
                        gl::vertex( to );
                    }
                }
            }
        }
        glEnd();
        
    }
    
    ForceMap OpticalFlow::getForceMap(){
        return mForceMap;
    }
    
    gl::Texture OpticalFlow::getOpticalFlowTexture(){
        return mOpticalFlowFBO.getTexture();
    }
    
    Vec2i OpticalFlow::getOpticalFlowTextureSize(){
        return mOpticalFlowFBO.getSize();
    }
    
    Rectf OpticalFlow::getOpticalFlowTextureBounds(){
        return mOpticalFlowFBO.getBounds();
    }
    
    //! private
    
    void OpticalFlow::_chooseFeatures( ::cv::Mat currentFrame ){
        ::cv::goodFeaturesToTrack( currentFrame, features, MAX_FEATURES, 0.005, 3.0 );
    }
    
    void OpticalFlow::_trackFeatures( ::cv::Mat currentFrame ){
        vector<float> errors;
        prevFeatures = features;
        if( ! features.empty() ){
            ::cv::calcOpticalFlowPyrLK( mPrevFrame, currentFrame, prevFeatures, features, featureStatuses, errors );
        }
    }
    
}}