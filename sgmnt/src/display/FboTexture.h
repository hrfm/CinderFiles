#pragma once

#include "Texture.h"
#include "EventDispatcher.h"

#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/Fbo.h"

namespace sgmnt{ namespace display{
    
    /*
     
     ci::qtime::MovieGlRef を用いて動画を表示するクラスです.
     動画の Texture への描画のみを行うシンプルなクラスです.
     
     */
    class FboTexture : public sgmnt::display::Texture, public sgmnt::events::EventDispatcher{
        
    public:
        
        FboTexture();
        FboTexture( int width, int height );
        FboTexture( ci::Vec2i size );
        ~FboTexture();
        
        virtual void init( ci::Vec2i size );
        
        virtual ci::gl::Fbo getFbo();
        
        virtual void update();
        
    protected:
        
        //virtual void _update();
        //virtual void _draw();
        
    private:
        
        ci::gl::Fbo mFbo;
        
    };
    
}}