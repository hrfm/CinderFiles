#include "MovieTexture.h"

namespace hrfm{ namespace display{
    
    // public:
    
    void MovieTexture::init( ci::fs::path filePath ){
        values.setValue("srcPath", filePath);
        init( ci::qtime::MovieGl::create( hrfm::fs::resolvePath(filePath) ) );
        addEventListener( hrfm::events::Event::REMOVED_FROM_STAGE, this, &MovieTexture::_onRemovedFromStage );
    }
    
    void MovieTexture::init( ::ci::qtime::MovieGlRef movieGlRef ){
        _init( movieGlRef );
    }
    
    ci::qtime::MovieGlRef MovieTexture::getMovieGlRef(){
        return _movieGlRef;
    }
    
    void MovieTexture::play( bool enableUpdateByTime ){
        _enableUpdateByTime = enableUpdateByTime;
        if( _movieGlRef != NULL ){
            
            _beforeTime = 0;
            _timeForUpdateByTime = 0.0;
            
            _movieGlRef->seekToStart();
            
            if( _enableUpdateByTime ){
                _movieGlRef->stop();
            }else if( !_movieGlRef->isPlaying() ){
                _movieGlRef->play();
            }
            
        }
    }
    void MovieTexture::play( ci::fs::path filePath, bool enableUpdateByTime ){
        values.setValue("srcPath", filePath);
        play( ci::qtime::MovieGl::create( hrfm::fs::resolvePath(filePath) ), enableUpdateByTime );
    }
    void MovieTexture::play( ::ci::qtime::MovieGlRef movieGlRef, bool enableUpdateByTime ){
        stop();
        _init( movieGlRef );
        play( enableUpdateByTime );
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
    
    void MovieTexture::setLoop(){
        _isLoop = true;
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
    
    ci::gl::TextureRef MovieTexture::getTexture(){
        if( _movieGlRef == NULL ){
            return NULL;
        }
        return _movieGlRef->getTexture();
    }
    
    void MovieTexture::_init( ci::qtime::MovieGlRef movieGlRef ){
        _beforeTime = 0.0f;
        _movieGlRef = movieGlRef;
        // --- Set Sound volume.
        if( isSilent() ){
            _movieGlRef->setVolume(0.0);
        }else{
            _movieGlRef->setVolume(_volume);
        }
        // --- Update Aspects.
        if( width == 0 ){
            width = _movieGlRef->getWidth();
        }
        if( height == 0 ){
            height = _movieGlRef->getHeight();
        }
        // --- Save properties.
        _duration    = _movieGlRef->getDuration();
        _secPerFrame = 1.0 / _movieGlRef->getFramerate();
        _totalFrames = _duration / _secPerFrame;
        
    }
    
    void MovieTexture::_update(){
        
        if( _movieGlRef == NULL ){
            return;
        }
        
        if( _enableUpdateByTime ){
            
            // TODO ちょっと強引なので修正が必要かも？
            
            int previousFrame = _timeForUpdateByTime / _secPerFrame;
            _timeForUpdateByTime += hrfm::app::SiAppInfo::getInstance().getElapsedSecondsFromBeforeUpdate();
            if( _duration < _timeForUpdateByTime ){
                _timeForUpdateByTime = _duration;
            }
            int nextFrame = _timeForUpdateByTime / _secPerFrame;
            
            while( previousFrame++ < nextFrame ){
                _movieGlRef->stepForward();
            }
            
            if( _timeForUpdateByTime == _duration ){
                _beforeTime = 0;
                dispatchEvent( new hrfm::events::Event( hrfm::events::Event::COMPLETE ) );
                if( _isLoop ){
                    play(true);
                }
                return;
            }
            
            _beforeTime = _timeForUpdateByTime;
            
        }else{
            
            float currentTime = _movieGlRef->getCurrentTime();
            
            if( _movieGlRef->isDone() || ( _movieGlRef->isPlaying() && ( currentTime == _duration || currentTime < _beforeTime ) ) ){
                _beforeTime = 0;
                dispatchEvent( new hrfm::events::Event( hrfm::events::Event::COMPLETE ) );
                return;
            }
            
            _beforeTime = currentTime;
            
        }
        
    }
    
    
    void MovieTexture::_onRemovedFromStage( hrfm::events::Event * evt ){
        stop();
    }
    
}}