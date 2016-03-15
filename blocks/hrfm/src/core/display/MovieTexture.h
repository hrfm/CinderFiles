#pragma once

#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/qtime/QuickTimeGl.h"

#include "hrfm.fs.h"
#include "TextureNode.h"

namespace hrfm{ namespace display{
    
    /*
     ci::qtime::MovieGlRef を用いて動画を表示するクラスです.
     動画の Texture への描画のみを行うシンプルなクラスです.
     */
    class MovieTexture : public TextureNode{
    
    public:
        
        MovieTexture():TextureNode(){};
        MovieTexture( ci::fs::path filePath ):TextureNode(){
            init( filePath );
        };
        MovieTexture( ci::qtime::MovieGlRef movieGlRef ):TextureNode(){
            init( movieGlRef );
        };
        ~MovieTexture(){};
        
        virtual void init( ci::fs::path filePath );
        virtual void init( ci::qtime::MovieGlRef movieGlRef );
        
        virtual ci::qtime::MovieGlRef getMovieGlRef();
        
        virtual void play( bool enableUpdateByTime = false );
        virtual void play( ci::fs::path filePath, bool enableUpdateByTime = false );
        virtual void play( ci::qtime::MovieGlRef movieGlRef, bool enableUpdateByTime = false );
        
        virtual void stop( bool seekToStart = true );
        
        virtual void  setVolume( float volume );
        virtual float getVolume();
        
        virtual bool isSilent();
        virtual void setSilent(bool flag);
        
        ci::gl::TextureRef getTexture();
        
    protected:
        
        virtual void _init( ci::qtime::MovieGlRef movieGlRef );
        virtual void _update();
        virtual void _onRemovedFromStage( hrfm::events::Event * evt );
        
        float  _beforeTime;
        float  _volume   = 0.0f;
        bool   _isSilent = false;
        int    _totalFrames;
        float  _secPerFrame;
        float  _duration;
        
        bool  _enableUpdateByTime  = false;
        float _timeForUpdateByTime = 0.0;
        
    private:
        
        ci::qtime::MovieGlRef _movieGlRef = NULL;
        
    };
    
}}