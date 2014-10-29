#include "MovieTexture.h"

using namespace ci;

namespace sgmnt{ namespace display{
    
    // public:
    
    MovieTexture::MovieTexture(){
        
        IDrawable();
        sgmnt::events::EventDispatcher();
        
        _beforeTime = 0.0f;
        
    };
    
    MovieTexture::MovieTexture( const string filePath ){
        MovieTexture();
        init(filePath);
    }
    
    MovieTexture::MovieTexture( ci::fs::path filePath ){
        MovieTexture();
        init(filePath);
    }
    
    MovieTexture::~MovieTexture(){
        
    }
    
    void MovieTexture::init( ci::fs::path filePath ){
        init(filePath.native());
    };
    
    void MovieTexture::init( const string filePath ){
        mMovieGlRef = ci::qtime::MovieGl::create( filePath );
    };
    
    bool MovieTexture::isDrawable(){
        return mMovieGlRef && mMovieGlRef->isPlaying();
    }
    
    ci::qtime::MovieGlRef MovieTexture::getMovieGlRef(){
        return mMovieGlRef;
    }
    
    ci::gl::Texture MovieTexture::getTexture(){
        return mMovieGlRef->getTexture();
    }
    
    void MovieTexture::_update(){
        float currentTime = mMovieGlRef->getCurrentTime();
        float duration = mMovieGlRef->getDuration();
        if( mMovieGlRef->isDone() || ( mMovieGlRef->isPlaying() && ( currentTime == duration || currentTime < _beforeTime ) ) ){
            dispatchEvent( new sgmnt::events::Event( sgmnt::events::Event::COMPLETE ) );
        }
        _beforeTime = currentTime;
    }
    
}}