#pragma once

#include "TextureNode.h"

#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/qtime/QuickTime.h"

namespace hrfm{ namespace display{
    
    /*
     
     ci::qtime::MovieGlRef を用いて動画を表示するクラスです.
     動画の Texture への描画のみを行うシンプルなクラスです.
     
     */
    class MovieTexture : public TextureNode{
        
    public:
        
        MovieTexture();
        MovieTexture( ci::fs::path filePath );
        ~MovieTexture();
        
        void init( ci::fs::path filePath );
        
        virtual ci::qtime::MovieGlRef getMovieGlRef();
        
        virtual void play();
        virtual void play( ci::fs::path filePath );
        virtual void play( ci::qtime::MovieGlRef & movieGlRef );
        
        virtual void stop();
        
        virtual ci::gl::Texture getTexture();
        
    protected:
        
        virtual void _update();
        
        float _beforeTime;
        
    private:
        
        ci::qtime::MovieGlRef _movieGlRef = NULL;
        
    };
    
}}