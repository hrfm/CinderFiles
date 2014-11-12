#include "MovieTexture.h"

using namespace ci;

namespace hrfm{ namespace display{
    
    // public:
    
    MovieTexture::MovieTexture(){
        hrfm::display::Texture();
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
        return Texture::isDrawable();
    }
    
    void MovieTexture::play(){
        if( mMovieGlRef ){
            _beforeTime = 0;
            mMovieGlRef->seekToStart();
            if( !mMovieGlRef->isPlaying() ){
                mMovieGlRef->play();
            }
        }
    }
    
    void MovieTexture::stop(){
        if( mMovieGlRef ){
            _beforeTime = 0;
            if( mMovieGlRef->isPlaying() ){
                mMovieGlRef->stop();
            }
            mMovieGlRef->seekToStart();
        }
    }
    
    ci::qtime::MovieGlRef MovieTexture::getMovieGlRef(){
        return mMovieGlRef;
    }
    
    void MovieTexture::_update(){
        if( mMovieGlRef->checkNewFrame() ){
            mTexture = mMovieGlRef->getTexture();
        }
        float currentTime = mMovieGlRef->getCurrentTime();
        float duration = mMovieGlRef->getDuration();
        if( mMovieGlRef->isDone() || ( mMovieGlRef->isPlaying() && ( currentTime == duration || currentTime < _beforeTime ) ) ){
            _beforeTime = 0;
            dispatchEvent( new hrfm::events::Event( hrfm::events::Event::COMPLETE ) );
        }else{
            _beforeTime = currentTime;
        }
    }
    
}}