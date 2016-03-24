#include "TessellationPlane.h"

namespace hrfm{ namespace display{
    
    
    void TessellationPlane::init( float num, ci::gl::TextureRef tex ){
        
        auto mBatch = ci::gl::VertBatch::create( GL_PATCHES );
        float w = 1.0 / num;
        float h = 1.0 / num;
        auto offset = vec3(-0.5,-0.5,0.0);
        
        //*
        for( int y=0; y<num; y++ ){
            
            for( int x=0; x<num; x++ ){
                
                mBatch->vertex( vec3( x*w    , y*h    , 0.0 ) + offset );
                mBatch->vertex( vec3( x*w    , (y+1)*h, 0.0 ) + offset );
                mBatch->vertex( vec3( (x+1)*w, y*h    , 0.0 ) + offset );
                
                mBatch->vertex( vec3( (x+1)*w, (y+1)*h, 0.0 ) + offset );
                mBatch->vertex( vec3( (x+1)*w, y*h    , 0.0 ) + offset );
                mBatch->vertex( vec3( x*w    , (y+1)*h, 0.0 ) + offset );
                
            }
            
        }
        ci::gl::patchParameteri( GL_PATCH_VERTICES, 3 );
        
        auto shader = ci::gl::GlslProg::create( ci::gl::GlslProg::Format()
                                               .vertex( DataLoader::load("Ts_vert.glsl") )
                                               .geometry(DataLoader::load("Ts_geo.glsl"))
                                               .tessellationCtrl( DataLoader::load( "Ts_tess_ctrl.glsl" ) )
                                               .tessellationEval( DataLoader::load( "Ts_tess_eval.glsl" ) )
                                               .fragment( DataLoader::load( "Ts_frag.glsl" ) )
                                               );
        
        _batchNode = new hrfm::display::VertBatchNode( mBatch, shader );
        if( tex != nullptr ){
            _batchNode->addTexture(tex);
        }
        this->addChild(_batchNode);
        
    }
    
    void TessellationPlane::setEnableWireframe( bool flag ){
        _batchNode->setEnableWireframe(flag);
    }
    
    void TessellationPlane::_update(){};
    
    void TessellationPlane::_draw(){};
    
}}