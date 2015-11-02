#include "MovieTexture.h"

namespace hrfm{ namespace display{
    
    // public:
    
    MovieTexture::MovieTexture(){
        TextureNode();
    };
    
    MovieTexture::MovieTexture( ci::fs::path filePath ){
        MovieTexture();
        init( filePath );
    };
    
    MovieTexture::~MovieTexture(){};
    
    void MovieTexture::init( ci::fs::path filePath ){
        _beforeTime = 0.0f;
        _srcPath    = filePath;
        _movieGlRef = ::ci::qtime::MovieGl::create( filePath );
    }
    
    void MovieTexture::play(){
        if( _movieGlRef ){
            _beforeTime = 0;
            _movieGlRef->seekToStart();
            if( !_movieGlRef->isPlaying() ){
                _movieGlRef->play();
            }
        }
    }
    
    void MovieTexture::play( ci::fs::path &filePath ){
        stop();
        init( filePath );
        play();
    }
    
    void MovieTexture::play( string filePath ){
        ci::fs::path path = filePath;
        play( path );
    }
    
    void MovieTexture::play( ::ci::qtime::MovieGlRef & movieGlRef ){
        stop();
        _movieGlRef = movieGlRef;
        play();
    }
    
    void MovieTexture::stop( bool seekToStart ){
        if( _movieGlRef ){
            _beforeTime = 0;
            if( _movieGlRef->isPlaying() ){
                _movieGlRef->stop();
            }
            if( seekToStart == true ){
                _movieGlRef->seekToStart();
            }
        }
    }
    
    void  MovieTexture::setVolume( float volume ){
        if( _isSilent ){
            _movieGlRef->setVolume(0.0);
        }else{
            _movieGlRef->setVolume(volume);
        }
    }
    
    float MovieTexture::getVolume(){
        if( _isSilent ){
            return 0;
        }else{
            return _movieGlRef->getVolume();
        }
    }
    
    bool MovieTexture::isSilent(){
        return _isSilent;
    }
    void MovieTexture::setSilent(bool flag){
        _isSilent = flag;
        if( _isSilent ){
            _movieGlRef->setVolume(0.0);
        }
    }
    
    ci::qtime::MovieGlRef MovieTexture::getMovieGlRef(){
        return _movieGlRef;
    }
    
    ci::gl::Texture MovieTexture::getTexture(){
        return _movieGlRef->getTexture();
    }
    
    void MovieTexture::_update(){
        float duration    = _movieGlRef->getDuration();
        float currentTime = _movieGlRef->getCurrentTime();
        if( _movieGlRef->isDone() || ( _movieGlRef->isPlaying() && ( currentTime == duration || currentTime < _beforeTime ) ) ){
            _beforeTime = 0;
            dispatchEvent( new hrfm::events::Event( hrfm::events::Event::COMPLETE ) );
        }else{
            _beforeTime = currentTime;
        }
    }
    
}}