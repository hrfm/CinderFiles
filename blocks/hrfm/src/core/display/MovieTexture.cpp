#include "MovieTexture.h"

namespace hrfm{ namespace display{
    
    // public:
    
    void MovieTexture::init( string filePathStr ){
        ci::fs::path path = filePathStr;
        init( path );
    }
    void MovieTexture::init( ci::fs::path filePath ){
        values.setValue("srcPath", filePath);
        init( ci::qtime::MovieGl::create( filePath ) );
    }
    void MovieTexture::init( ::ci::qtime::MovieGlRef movieGlRef ){
        _init( movieGlRef );
    }
    
    ci::qtime::MovieGlRef MovieTexture::getMovieGlRef(){
        return _movieGlRef;
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
    void MovieTexture::play( string filePathStr ){
        ci::fs::path path = filePathStr;
        play( path );
    }
    void MovieTexture::play( ci::fs::path filePath ){
        values.setValue("srcPath", filePath);
        play( ci::qtime::MovieGl::create( filePath ) );
    }
    void MovieTexture::play( ::ci::qtime::MovieGlRef movieGlRef ){
        stop();
        _init( movieGlRef );
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
        _volume = volume;
        if( _isSilent ){
            _movieGlRef->setVolume(0.0);
        }else{
            _movieGlRef->setVolume(volume);
        }
    }
    
    float MovieTexture::getVolume(){
        return _volume;
    }
    
    bool MovieTexture::isSilent(){
        return _isSilent;
    }
    void MovieTexture::setSilent(bool flag){
        _isSilent = flag;
        if( _isSilent ){
            _movieGlRef->setVolume(0.0);
        }else{
            _movieGlRef->setVolume(_volume);
        }
    }
    
    /*
    !!!!!!! textureRef の参照を保持しているから大丈夫なはず・・・
    ci::gl::TextureRef MovieTexture::getTexture(){
        return _movieGlRef->getTexture();
    }
    //*/
    
    void MovieTexture::_init( ci::qtime::MovieGlRef movieGlRef ){
        _beforeTime = 0.0f;
        _movieGlRef = movieGlRef;
        if( isSilent() ){
            _movieGlRef->setVolume(0.0);
        }else{
            _movieGlRef->setVolume(_volume);
        }
        _texture = _movieGlRef->getTexture();
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