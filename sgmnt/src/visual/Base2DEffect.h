#pragma once

#include "cinder/app/AppBasic.h"

#include "cinder/Xml.h"

#include "cinder/gl/gl.h"
#include "cinder/gl/Fbo.h"
#include "Cinder/gl/GlslProg.h"

#include "Utils.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace sgmnt::utils;

namespace sgmnt { namespace visual{
        
        // --- Event for OSC. ---
        
        class Base2DEffect{
            
            public:
                
                Base2DEffect(){};
            
                // 最も基本的な初期化を行います.
                // どのシェーダを使うかはクラスに任せます.
                void setup( Vec2i size ){
                    
                    gl::Fbo::Format format;
                    mFbo = gl::Fbo( size.x, size.y, format );
                    
                    cout << "create FBO " << size << endl;
                    
                    // Calcurate Aspect Ratio.
                    
                    mAspect = getAspectRatio( size );
                    
                    // Create Shader.
                    
                    try {
                        mShader = gl::GlslProg( getVertexShader(), getFragmentShader() );
                    }catch( ci::gl::GlslProgCompileExc &exc ) {
                        cout << "Shader compile error: " << endl;
                        cout << exc.what();
                    }catch( ... ) {
                        cout << "Unable to load shader" << endl;
                    }
                }
            
                virtual gl::Texture affect( gl::Texture tex ){
                    
                    Area viewport = gl::getViewport();
                    
                    Vec2f windowSize   = mFbo.getSize();
                    Vec2f resolution   = Vec2f( mFbo.getWidth(), mFbo.getHeight() );
                    
                    mFbo.bindFramebuffer();
                        gl::pushMatrices();
                            gl::setViewport( mFbo.getBounds() );
                            gl::setMatricesWindow( mAspect, false );
                            gl::clear();
                            mShader.bind();
                                tex.bind(0);
                                mShader.uniform( "tex"       , 0 );
                                mShader.uniform( "time"      , (float)getElapsedSeconds() );
                                mShader.uniform( "windowSize", windowSize );
                                mShader.uniform( "resolution", resolution );
                                prepare();
                                gl::drawSolidRect( Rectf( 0, 0, mAspect.x, mAspect.y ) );
                                clear();
                                tex.unbind();
                            mShader.unbind();
                        gl::popMatrices();
                    mFbo.unbindFramebuffer();
                    
                    gl::setViewport( viewport );
                    
                    return mFbo.getTexture();
                    
                };
            
                void draw( Rectf bounds ){
                    gl::draw( getTexture(), bounds );
                }
            
                gl::Texture getTexture(){
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
                
            protected :
            
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
            
                gl::Fbo mFbo;
                gl::GlslProg mShader;
            
                Vec2i mAspect;
            
                float mCycle;
            
            private:
                
        };
        
    }
    
}