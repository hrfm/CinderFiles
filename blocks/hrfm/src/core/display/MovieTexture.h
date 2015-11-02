#pragma once

#include "TextureNode.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"

#ifndef _QT__
#include "cinder/qtime/QuickTime.h"
#endif

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
        virtual void play( ci::fs::path &filePath );
        virtual void play( string filePath );
        virtual void play( ci::qtime::MovieGlRef & movieGlRef );
        
        virtual void stop( bool seekToStart = true );
        
        virtual void setVolume( float volume );
        virtual float getVolume();
        
        virtual bool isSilent();
        virtual void setSilent(bool flag);
        
        virtual ci::gl::Texture getTexture();
        
    protected:
        
        virtual void _update();
        
        float _beforeTime;
        bool  _isSilent = false;
        
    private:
        
        ci::qtime::MovieGlRef _movieGlRef = NULL;
        
    };
    
}}