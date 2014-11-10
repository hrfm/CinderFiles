#pragma once

#include "Texture.h"

#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/Fbo.h"
#include "cinder/Camera.h"

namespace hrfm{ namespace display{
    
    /*
     
     ci::qtime::MovieGlRef を用いて動画を表示するクラスです.
     動画の Texture への描画のみを行うシンプルなクラスです.
     
     */
    class FboTexture : public hrfm::display::Texture{
        
    public:
        
        FboTexture();
        FboTexture( int width, int height );
        FboTexture( ci::Vec2i size );
        ~FboTexture();
        
        virtual void init( ci::Vec2i size );
        
        virtual ci::gl::Fbo getFbo();
        
        virtual ci::gl::Texture getTexture();
        
        virtual void update();
        
    protected:
        
        ci::CameraPersp _getFboCameraPersp();
        
        ci::gl::Fbo mFbo;
        
        //virtual void _update();
        //virtual void _draw();
        
    private:
        
    };
    
}}