#pragma once

#include "VisualBase.h"
#include "cinder/gl/Texture.h"

using namespace ci;
using namespace ci::app;
using namespace std;

using namespace hrfm;
using namespace hrfm::visual;

namespace hrfm{ namespace visual{
    
    class ShaderVisual : public VisualBase{
        
        public :
        
            ShaderVisual( AppBase * app, Vec2i resolution, string fragment ){
                VisualBase();
                setup( app, resolution );
                addVisual(fragment);
            };
            
            ShaderVisual( AppBase * app, Vec2i resolution, vector<string> fragments ){
                VisualBase();
                setup( app, resolution );
                vector<string>::iterator it = fragments.begin();
                while( it != fragments.end() ){
                    addVisual( (*it) );
                    it++;
                }
            };
            
            void setup( AppBase * app, Vec2i resolution ){
                cout << "- Setup ShaderVisual " << resolution << endl;
                VisualBase::setup( app, resolution );
            };
            
            void addVisual( string fragment ){
                cout << "addVisual : " << fragment << endl;
                try {
                    gl::GlslProg shader = gl::GlslProg(
                                                       loadResource("../resources/vert.glsl"),
                                                       loadResource(fragment)
                                                       );
                    mShaders.push_back( shader );
                }catch( ci::gl::GlslProgCompileExc &exc ) {
                    cout << "Shader compile error: " << endl;
                    cout << exc.what();
                }catch( ... ) {
                    cout << "Unable to load shader" << endl;
                }
            }
            
            void update(){
                
                mTextures.clear();
                if( mAppBase->useCapture ){
                    mTextures.push_back( mAppBase->captureInput.getCaptureTexture() );
                }
                if( mAppBase->captureInput.useOpticalFlow ){
                    mTextures.push_back( mAppBase->captureInput.getForceMap().getForceTexture() );
                }
                
                float iGlobalTime = (float)getElapsedSeconds();
                Vec2f iResolution = Vec2f( mOutputFbo.getWidth(), mOutputFbo.getHeight() );
                Vec2f iMouse      = Vec2f( 0, 0 );
                
                float average = mAppBase->audioInput.getAudioManagerFFTAverage();
                float fader0  = mAppBase->getFaderValueAt(4) * mAppBase->getFaderValueAt(12) * 3.0;
                float fader1  = mAppBase->getFaderValueAt(5) * mAppBase->getFaderValueAt(13) * 3.0;
                average = fader0 + average * fader1;
                
                vector<gl::GlslProg>::iterator it = mShaders.begin();
                const std::vector<gl::GlslProg>::const_iterator & end = mShaders.end();
                
                vector<gl::Texture>::iterator texIt;
                const std::vector<gl::Texture>::const_iterator & texItEnd = mTextures.end();
                
                gl::enableAdditiveBlending();
                
                mOutputFbo.bindFramebuffer();
                
                    gl::pushMatrices();
                    gl::setMatricesWindow( mOutputFbo.getSize(), false );
                
                    gl::clear();
                    gl::color( ColorA( 1.0, 1.0, 1.0, 1.0 ) );
                    
                    for (it = mShaders.begin(); it != end; ++it) {
                        
                        gl::GlslProg shader = (*it);
                        
                        shader.bind();
                            // --- Bind Textures if needed. ---
                            int bindIndex = 0;
                            if( mAppBase->useCapture ){
                                mAppBase->captureInput.getCaptureTexture().bind(bindIndex);
                                shader.uniform( "iChannel"+toString(bindIndex), bindIndex++ );
                            }
                            for( texIt = mTextures.begin(); texIt != texItEnd; ++texIt ) {
                                (*texIt).bind( bindIndex );
                                shader.uniform( "iChannel"+toString(bindIndex), bindIndex++ );
                            }
                            // ---
                            shader.uniform( "iResolution", iResolution );
                            shader.uniform( "iGlobalTime", iGlobalTime );
                            shader.uniform( "iMouse"     , iMouse );
                            shader.uniform( "iFFT"       , mAppBase->audioInput.fft2, 64 );
                            shader.uniform( "iFFTAverage", average );
                            shader.uniform( "iAudioGain" , mAppBase->audioInput.getAudioManagerGain() );
                            gl::drawSolidRect( mOutputFbo.getBounds() );
                            // --- Unbind Textures if needed.
                            if( mAppBase->useCapture ){
                                mAppBase->captureInput.getCaptureTexture().unbind();
                            }
                            for( texIt = mTextures.begin(); texIt != texItEnd; ++texIt ) {
                                (*texIt).unbind();
                            }
                            // ---
                        shader.unbind();
                        
                    }
                
                    gl::popMatrices();
                
                mOutputFbo.unbindFramebuffer();
                
                gl::disableAlphaBlending();
                
            }
            
            void draw(){};
            
        private :
        
            vector<gl::Texture>     mTextures;
            vector<gl::GlslProg>    mShaders;
        
    };
    
}}