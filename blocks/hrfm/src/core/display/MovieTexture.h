#pragma once

#include "Texture.h"
#include "EventDispatcher.h"

#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/qtime/QuickTime.h"

namespace hrfm{ namespace display{
    
    /*
     
     ci::qtime::MovieGlRef を用いて動画を表示するクラスです.
     動画の Texture への描画のみを行うシンプルなクラスです.
     
     */
    class MovieTexture : public hrfm::display::Texture{
        
    public:
        
        MovieTexture();
        MovieTexture( const string filePath );
        MovieTexture( ci::fs::path filePath );
        ~MovieTexture();
        
        void init( const string filePath );
        void init( ci::fs::path filePath );
        
        virtual bool isDrawable();
        
        virtual ci::qtime::MovieGlRef getMovieGlRef();
        
        virtual void play();
        virtual void stop();
        
    protected:
        
        virtual void _update();
        
        float _beforeTime;
        
    private:
        
        ci::qtime::MovieGlRef mMovieGlRef;
        
    };
    
}}