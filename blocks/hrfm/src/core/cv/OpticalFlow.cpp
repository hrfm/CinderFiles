#include "OpticalFlow.h"

using namespace std;
using namespace ci;

namespace hrfm{ namespace cv{
    
    OpticalFlow::OpticalFlow( ivec2 textureSize, int forceMapScale ){
        
        // --- Create FBO. ---
        
        ci::gl::Fbo::Format captureFormat;
        mOpticalFlowFBO    = ci::gl::Fbo::create( textureSize.x, textureSize.y, captureFormat );
        mOpticalFlowBounds = mOpticalFlowFBO->getBounds();
        
        // --- Create ForceMap. ---
        
        mForceMap.setup( 16 * forceMapScale, 9 * forceMapScale );
        
    }
    
    void OpticalFlow::update( ci::gl::TextureRef texture, float bias, float frameRate ){
        
        std::pair<ivec2,ivec2> viewport = ci::gl::getViewport();
        
        mOpticalFlowFBO->bindFramebuffer();
        {
            ci::gl::pushMatrices();
            ci::gl::viewport( ivec2(0), mOpticalFlowFBO->getSize() );
            ci::gl::setMatricesWindow( mOpticalFlowFBO->getWidth(), mOpticalFlowBounds.getHeight(), false );
            {
                ci::gl::clear();
                ci::gl::color(1.0f,1.0f,1.0f);
                ci::gl::draw( texture, mOpticalFlowBounds );
            }
            ci::gl::popMatrices();
        }
        mOpticalFlowFBO->unbindFramebuffer();
        
        ci::gl::viewport(viewport);
        
        /* !!!!!!!!!!
        if( 1.0 / frameRate < ci::app::getElapsedSeconds() - recentTime ){
            
            ::cv::Mat currentFrame( toOcv( mOpticalFlowFBO->getColorTexture(), CV_8UC1 ) );
            
            if( mPrevFrame.data ) {
                if( features.empty() || ci::app::getElapsedFrames() % 3 == 0 ){
                    // pick new features once every 30 frames, or the first frame
                    _chooseFeatures( mPrevFrame );
                }
                _trackFeatures( currentFrame );
            }
            
            mPrevFrame = currentFrame;
            
            // --- Update ForceMap. ---
            
            vec2 from, to, size( mOpticalFlowFBO->getSize() );
            float dist;
            
            int max = features.size();
            for( size_t idx = 0; idx < max; ++idx ) {
                if( featureStatuses[idx] ) {
                    from = fromOcv( features[idx] );
                    to   = fromOcv( prevFeatures[idx] );
                    dist = ci::distance(from,to);
                    if( 10.f < dist && dist < 50.0f ){
                        mForceMap.addForce( from / size, ( to - from ) * bias );
                    }
                }
            }
            
            recentTime = ci::app::getElapsedSeconds();
            
        }
        //*/
        
        mForceMap.update();
        
    }
    
    void OpticalFlow::draw(){
        draw( mOpticalFlowBounds );
    }
    
    void OpticalFlow::draw( Rectf bounds ){
        
        float scaleX = bounds.getWidth() / mOpticalFlowBounds.getWidth();
        float scaleY = bounds.getHeight() / mOpticalFlowBounds.getHeight();
        
        vec2 from, to;
        int  length = features.size();
        
        ci::gl::lineWidth(2.0);
        glDisable( GL_TEXTURE_2D );
        
        /*
        vector<::cv::Point2f>::const_iterator featureIt, featureEnd;
        glColor4f( 1, 1, 0, 0.5f );
        // draw all the old points
        for( featureIt = prevFeatures.begin(), featureEnd = prevFeatures.end(); featureIt != featureEnd; ++featureIt ){
            gl::drawStrokedCircle( fromOcv( *featureIt ), 1 );
        }
        // draw all the new points
        for( featureIt = features.begin(), featureEnd = features.end(); featureIt != featureEnd; ++featureIt ){
            gl::drawSolidCircle( fromOcv( *featureIt ), 1 );
        }
        //*/
        
        // draw the lines connecting them
        ci::gl::color( ColorA( 1.0f, 1.0f, 1.0f, 1.0f ) );
        
        ci::gl::begin( GL_LINES );
        {
            
            for( size_t idx = 0; idx < length; ++idx ) {
                if( featureStatuses[idx] ) {
                    
                    from = fromOcv( features[idx] );
                    from.x *= scaleX;
                    from.y *= scaleY;
                    
                    to   = fromOcv( prevFeatures[idx] );
                    to.x *= scaleX;
                    to.y *= scaleY;
                    
                    if( distance(from,to) < 40.0 ){
                        ci::gl::vertex( from );
                        ci::gl::vertex( to );
                    }
                    
                }
            }
            
        }
        ci::gl::end();
        
    }
    
    ForceMap OpticalFlow::getForceMap(){
        return mForceMap;
    }
    
    ForceMap * OpticalFlow::getForceMapPtr(){
        return &mForceMap;
    }
    
    ci::gl::TextureRef OpticalFlow::getTexture(){
        return mOpticalFlowFBO->getColorTexture();
    }
    
    ivec2 OpticalFlow::getSize(){
        return mOpticalFlowFBO->getSize();
    }
    
    Rectf OpticalFlow::getBounds(){
        return mOpticalFlowFBO->getBounds();
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