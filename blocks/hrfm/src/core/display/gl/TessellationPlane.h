#pragma once

#include "hrfm.h"
#include "ExFbo.h"
#include "cinder/gl/gl.h"
#include "DataLoader.h"

namespace hrfm{ namespace display{
    
    class TessellationPlane : public hrfm::display::DisplayNode{
        
    public:
        
        TessellationPlane( float tessLevel = 50.0 ):hrfm::display::DisplayNode(){
            setup( tessLevel );
        }
        
        void setup( float tessLevel ){
            
            // ---------------subdivisions-----------------------------------------------------
            
            float numX = 4.0;
            float numY = 4.0;
            _batch = ci::gl::VertBatch::create( GL_PATCHES );
            float w = 1.0 / numX;
            float h = 1.0 / numY;
            auto offset = vec3(-0.5,-0.5,0.0);
            for( int y=0; y<numY; y++ ){
                for( int x=0; x<numX; x++ ){
                    _batch->vertex( vec3( x*w    , y*h    , 0.0 ) + offset );
                    _batch->vertex( vec3( (x+1)*w, y*h    , 0.0 ) + offset );
                    _batch->vertex( vec3( x*w    , (y+1)*h, 0.0 ) + offset );
                    _batch->vertex( vec3( (x+1)*w, (y+1)*h, 0.0 ) + offset );
                }
            }
            ci::gl::patchParameteri( GL_PATCH_VERTICES, 4 );
            
            // --------------------------------------------------------------------
            
            _shader = ci::gl::GlslProg::create( ci::gl::GlslProg::Format()
                                              .vertex          ( io::DataLoader::load( "Ts_vert.glsl") )
                                              .tessellationCtrl( io::DataLoader::load( "Ts_tess_ctrl.glsl" ) )
                                              .tessellationEval( io::DataLoader::load( "Ts_tess_eval.glsl" ) )
                                              .geometry        ( io::DataLoader::load( "Ts_geo.glsl") )
                                              .fragment        ( io::DataLoader::load( "Ts_frag.glsl" ) )
                                              );
            
            //_shader->uniform( "uTessLevelInner", tessLevel );
            //_shader->uniform( "uTessLevelOuter", tessLevel );
            
            _batchNode = new hrfm::display::VertBatchNode( _batch, _shader );
            
        }
        
        void setTexture( ci::gl::TextureRef tex ){
            this->tex = tex;
        }
        
        void setTexture( hrfm::gl::ExFboRef fbo ){
            this->fbo = fbo;
        }
        
        ci::gl::TextureRef tex = NULL;
        hrfm::gl::ExFboRef fbo = NULL;
        
    protected:
        
        void _update(){}
        
        void _draw(){
            
            
            if( fbo != NULL ){
                ci::gl::draw(fbo->getTexture());
            }
            
            ci::gl::enableAdditiveBlending();
            {
                
                if( tex != NULL ){
                    tex->bind();
                    _shader->uniform( "tex", 0 );
                }else if( fbo != NULL ){
                    fbo->getTexture()->bind();
                    cout << "fbo tex" << endl;
                    _shader->uniform( "tex", 0 );
                }
                
                ci::gl::enableDepthRead();
                ci::gl::enableDepthWrite();
                {
                    _batchNode->draw();
                }
                ci::gl::disableDepthRead();
                ci::gl::disableDepthWrite();
                
                if( tex != NULL ){
                    tex->unbind();
                }else if( fbo != NULL ){
                    fbo->getTexture()->unbind();
                }
                
            }
            ci::gl::disableAlphaBlending();
        }
        
        // -----------
        
        vec3 rotation = vec3(0.0);
        
        float fader[8];
        float knob[8];
        
        ci::gl::GlslProgRef  _shader;
        ci::gl::VertBatchRef _batch;
        hrfm::display::VertBatchNode * _batchNode;
        
    };

}}
