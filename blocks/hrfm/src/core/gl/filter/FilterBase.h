#pragma once

#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Fbo.h"
#include "Cinder/gl/GlslProg.h"
#include "cinder/Xml.h"
#include "SiFboFactory.h"
#include "ShaderFactory.h"
#include "Utils.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace hrfm::utils;

namespace hrfm { namespace gl{
    
    // --- Event for OSC. ---
    
    class FilterBase{
        
    public:
        
        FilterBase(){};
        
        // 最も基本的な初期化を行います.
        // どのシェーダを使うかはクラスに任せます.
        void setup( Vec2i size ){
            
            // Calcurate Aspect Ratio.
            mAspect = getAspectRatio( size );
            
            // Create Fbo.
            mFbo = *SiFboFactory::getInstance().create( size.x, size.y, false );
            
            // Create Shader.
            mShader = ShaderFactory::create( getVertexShader(), getFragmentShader() );
            
        }
        
        virtual ci::gl::Texture affect( ci::gl::Texture tex ){
            
            Area viewport = ci::gl::getViewport();
            
            Vec2f windowSize   = mFbo.getSize();
            Vec2f resolution   = Vec2f( mFbo.getWidth(), mFbo.getHeight() );
            
            mFbo.bindFramebuffer();
            {
                ci::gl::pushMatrices();
                {
                    //*
                    ci::gl::setViewport( mFbo.getBounds() );
                    ci::gl::setMatricesWindow( mAspect, false );
                    /*/
                    ci::gl::setViewport( mFbo.getBounds() );
                    ci::gl::setMatricesWindow( mFbo.getWidth(), mFbo.getHeight(), false );
                    //*/
                    ci::gl::clear();
                    mShader.bind();
                    {
                        tex.bind(0);
                        mShader.uniform( "tex"       , 0 );
                        mShader.uniform( "time"      , (float)getElapsedSeconds() );
                        mShader.uniform( "windowSize", windowSize );
                        mShader.uniform( "resolution", resolution );
                        prepare();
                        ci::gl::drawSolidRect( Rectf( 0, 0, mAspect.x, mAspect.y ) );
                        clear();
                        tex.unbind();
                    }
                    mShader.unbind();
                }
                ci::gl::popMatrices();
            }
            mFbo.unbindFramebuffer();
            
            ci::gl::setViewport( viewport );
            
            return mFbo.getTexture();
            
        };
        
        void draw( Rectf bounds ){
            ci::gl::draw( getTexture(), bounds );
        }
        
        ci::gl::Texture getTexture(){
            return mFbo.getTexture();
        }
        
        void bindTexture( int index ){
            mFbo.bindTexture( index );
        }
        
        void unbindTexture(){
            mFbo.unbindTexture();
        }
        
        int getWidth(){
            return mFbo.getWidth();
        }
        
        int getHeight(){
            return mFbo.getHeight();
        }
        
        Rectf getBounds(){
            return mFbo.getBounds();
        }
        
    protected:
        
        virtual DataSourceRef getVertexShader(){
            return loadResource("../resources/simple_vert.glsl");
        }
        
        virtual DataSourceRef getFragmentShader(){
            return loadResource("../resources/simple_frag.glsl");
        }
        
        // prepare shader, texture, and more. before drawSolidRect to FrameBuffer.
        virtual void prepare(){}
        
        // clear shader, texture, and more. after drawSolidRect to FrameBuffer.
        virtual void clear(){}
        
        ci::gl::Fbo mFbo;
        ci::gl::GlslProg mShader;
        
        Vec2i mAspect;
        
        float mCycle;
        
    private:
        
    };
    
}
    
}