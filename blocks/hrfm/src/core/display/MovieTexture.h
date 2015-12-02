#pragma once

#include "TextureNode.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/qtime/QuickTimeGl.h"

namespace hrfm{ namespace display{
    
    /*
     ci::qtime::MovieGlRef を用いて動画を表示するクラスです.
     動画の Texture への描画のみを行うシンプルなクラスです.
     */
    class MovieTexture : public TextureNode{
    
    public:
        
        MovieTexture():TextureNode(){};
        MovieTexture( string filePathStr ):TextureNode(){
            init( filePathStr );
        };
        MovieTexture( ci::fs::path filePath ):TextureNode(){
            init( filePath );
        };
        MovieTexture( ci::qtime::MovieGlRef movieGlRef ):TextureNode(){
            init( movieGlRef );
        };
        ~MovieTexture(){};
        
        virtual void init( string filePathStr );
        virtual void init( ci::fs::path filePath );
        virtual void init( ci::qtime::MovieGlRef movieGlRef );
        
        virtual ci::qtime::MovieGlRef getMovieGlRef();
        
        virtual void play();
        virtual void play( string filePathStr );
        virtual void play( ci::fs::path filePath );
        virtual void play( ci::qtime::MovieGlRef movieGlRef );
        
        virtual void stop( bool seekToStart = true );
        
        virtual void  setVolume( float volume );
        virtual float getVolume();
        
        virtual bool isSilent();
        virtual void setSilent(bool flag);
        
        virtual ci::gl::TextureRef getTexture();
        
    protected:
        
        virtual void _init( ci::qtime::MovieGlRef movieGlRef );
        virtual void _update();
        
        float _beforeTime;
        float _volume   = 1.0f;
        bool  _isSilent = false;
        
    private:
        
        ci::qtime::MovieGlRef _movieGlRef = NULL;
        
    };
    
}}