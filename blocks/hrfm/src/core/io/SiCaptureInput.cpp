#include "SiCaptureInput.h"

namespace hrfm { namespace io{
    
    //! public
    
    void SiCaptureInput::showAllDevices(){
        // print the devices
        cout << endl << "SiCaptureInput::showAllDevices()" << endl;
        for( auto device = ci::Capture::getDevices().begin(); device != ci::Capture::getDevices().end(); ++device ) {
            cout << "- " << (*device)->getName() << " " << endl;
        }
        cout << endl;
    }
    
    ci::CaptureRef SiCaptureInput::createRef( int width, int height, string deviceName, int cacheLength ){
        
        if( _captureRefMap.find( deviceName ) != _captureRefMap.end() ){
            return getCaptureRef(deviceName);
        }
        
        ci::CaptureRef ref;
        
        try {
            
            if( deviceName == "*" ){
                ref = ci::Capture::create( width, height );
            }else{
                ref = ci::Capture::create( width, height, ci::Capture::findDeviceByName(deviceName) );
            }
            ref->start();
            
            _captureRefMap[deviceName] = ref;
            
            // --- texture cache.
            
            _textureCacheLengthMap[deviceName] = max( 2, cacheLength );
            _textureCacheVectorMap[deviceName] = vector<ci::gl::Texture>();
            _textureCacheVectorMap[deviceName].push_back( ci::gl::Texture(1, 1) );
            
        }catch( ... ) {
            cout << "Failed to initialize capture" << std::endl;
        }
        
        return ref;
        
    }
    
    void SiCaptureInput::setDefaultDeviceName( string deviceName ){
        _defaultDeviceName = deviceName;
    }
    
    //! --- サイズの取得
    
    ci::Vec2i SiCaptureInput::getSize(){
        return getSize( _defaultDeviceName );
    }
    ci::Vec2i SiCaptureInput::getSize( string deviceName ){
        return _captureRefMap[deviceName]->getSize();
    }
    
    //! --- Capture の更新
    
    
    void SiCaptureInput::update(){
        update( _defaultDeviceName );
    }
    void SiCaptureInput::update( string deviceName ){
        _updateTexture( deviceName );
        _updateDiffTexture( deviceName );
    }
    
    // --- CaptureRef の取得
    
    ci::CaptureRef SiCaptureInput::getCaptureRef(){
        return getCaptureRef( _defaultDeviceName );
    }
    ci::CaptureRef SiCaptureInput::getCaptureRef( string deviceName ){
        return _captureRefMap[deviceName];
    }
    
    // --- Surface の取得
    
    ci::Surface SiCaptureInput::getSurface(){
        return getSurface( _defaultDeviceName );
    }
    ci::Surface SiCaptureInput::getSurface( string deviceName ){
        return _captureRefMap[deviceName]->getSurface();
    }
    
    // --- Texture の取得
    
    ci::gl::Texture SiCaptureInput::getTexture( int cacheAt ){
        return getTexture( _defaultDeviceName, cacheAt );
    }
    ci::gl::Texture SiCaptureInput::getTexture( string deviceName, int cacheAt ){
        return *getTexturePtr( deviceName, cacheAt );
    }
    
    // --- 指定したサイズで Texture を取得
    
    ci::gl::Texture SiCaptureInput::getResizedTexture( Vec2i size, int cacheAt ){
        return getResizedTexture( size, _defaultDeviceName, cacheAt );
    }
    ci::gl::Texture SiCaptureInput::getResizedTexture( Vec2i size, string deviceName, int cacheAt ){
        return hrfm::gl::resizeTexture( getTexturePtr(deviceName, cacheAt), size );
    }
    
    // --- Texture のポインタを取得
    
    ci::gl::Texture * SiCaptureInput::getTexturePtr( int cacheAt ){
        return getTexturePtr( _defaultDeviceName, cacheAt );
    }
    ci::gl::Texture * SiCaptureInput::getTexturePtr( string deviceName, int cacheAt ){
        int numCached = getNumCached(deviceName);
        if( numCached < cacheAt + 1 ){
            cacheAt = numCached - 1;
        }
        return &_textureCacheVectorMap[deviceName].at( _textureCacheVectorMap[deviceName].size() - (cacheAt+1) );
    }
    
    // --- 前フレームとの差分テクスチャを取得
    
    ci::gl::Texture SiCaptureInput::getDiffTexture(){
        return getDiffTexture( _defaultDeviceName );
    }
    ci::gl::Texture SiCaptureInput::getDiffTexture( string deviceName ){
        if( _diffFboMap.find(deviceName) == _diffFboMap.end() ){
            ci::Vec2i size = getCaptureRef(deviceName)->getSize();
            ci::gl::Fbo::Format format;
            ci::gl::Fbo fbo = ci::gl::Fbo( size.x, size.y, format );
            _diffFboMap[deviceName] = fbo;
        }
        return _diffFboMap[deviceName].getTexture();
    }
    
    // --- 何フレーム分キャプチャを行ったかを取得
    
    int SiCaptureInput::getNumCached(){
        return getNumCached( _defaultDeviceName );
    }
    int SiCaptureInput::getNumCached( string deviceName ){
        return _textureCacheVectorMap[deviceName].size();
    }
    
    // --- 何フレーム分キャッシュするかの上限を取得
    
    int SiCaptureInput::getCacheLength(){
        return getCacheLength( _defaultDeviceName );
    }
    int SiCaptureInput::getCacheLength( string deviceName ){
        return _textureCacheLengthMap[deviceName];
    }
    
    //! protected
    
    ci::gl::Texture SiCaptureInput::_updateTexture( string deviceName ){
        if( _beforeFrameMap.find(deviceName) == _beforeFrameMap.end() ){
            _beforeFrameMap[deviceName] = 0;
        }else{
            int currentFrame = getElapsedFrames();
            int beforeFrame  = _beforeFrameMap[deviceName];
            if( beforeFrame < currentFrame ){
                _beforeFrameMap[deviceName] = currentFrame;
                vector<ci::gl::Texture> * vec = &_textureCacheVectorMap[deviceName];
                if( getCaptureRef(deviceName)->checkNewFrame() ){
                    vec->push_back( ci::gl::Texture( getSurface( deviceName ) ) );
                }else{
                    vec->push_back( getTexture(deviceName) );
                }
                if( _textureCacheLengthMap[deviceName] < vec->size() ){
                    vec->erase(vec->begin());
                }
            }
        }
        return getTexture(deviceName);
    }
    
    ci::gl::Texture SiCaptureInput::_updateDiffTexture( string deviceName ){
        
        if( _beforeTextureBeforeFrameMap.find(deviceName) == _beforeTextureBeforeFrameMap.end() ){
            _beforeTextureBeforeFrameMap[deviceName] = 0;
        }
        
        int currentFrame = getElapsedFrames();
        int beforeFrame  = _beforeTextureBeforeFrameMap[deviceName];
        
        vector<ci::gl::Texture> * vec = &_textureCacheVectorMap[deviceName];
        
        if( beforeFrame != currentFrame ){
            
            if( 1 < vec->size() ){
                
                Area viewport = ci::gl::getViewport();
                
                if( _diffFboMap.find(deviceName) == _diffFboMap.end() ){
                    ci::Vec2i size = getCaptureRef(deviceName)->getSize();
                    ci::gl::Fbo::Format format;
                    ci::gl::Fbo fbo = ci::gl::Fbo( size.x, size.y, format );
                    _diffFboMap[deviceName] = fbo;
                }
                
                ci::gl::Texture * tex   = getTexturePtr(deviceName);
                ci::gl::Texture * b4Tex = getTexturePtr(deviceName,1);
                ci::gl::Fbo     fbo     = _diffFboMap[deviceName];
                
                // 前フレームとの差分を取得する.
                fbo.bindFramebuffer();
                {
                    ci::gl::pushMatrices();
                    ci::gl::setViewport( fbo.getBounds() );
                    ci::gl::setMatricesWindow( fbo.getSize(), false );
                    {
                        ci::gl::clear();
                        ci::gl::color(1.0f,1.0f,1.0f);
                        b4Tex->bind(0);
                        tex->bind(1);
                        _diffShader.bind();
                        {
                            _diffShader.uniform("tex0", 0);
                            _diffShader.uniform("tex1", 1);
                            _diffShader.uniform("resolution", Vec2f(fbo.getSize()) );
                            ci::gl::drawSolidRect( fbo.getBounds() );
                        }
                        _diffShader.unbind();
                        tex->unbind();
                        b4Tex->unbind();
                    }
                    ci::gl::popMatrices();
                }
                fbo.unbindFramebuffer();
                
                ci::gl::setViewport(viewport);
                
            }
            
            _beforeTextureBeforeFrameMap[deviceName] = currentFrame;
            
        }
        
        return getDiffTexture();
        
    }
    
}}