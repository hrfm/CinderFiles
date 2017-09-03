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
            
            if( deviceName == "" ){
                deviceName = _defaultDeviceName;
            }
            
            if( deviceName == "*" ){
                ref = ci::Capture::create( width, height );
            }else{
                ref = ci::Capture::create( width, height, ci::Capture::findDeviceByName(deviceName) );
            }
            
            ref->start();
            
            _captureRefMap[deviceName] = ref;
            
            // --- texture cache.
            
            _textureCacheLengthMap[deviceName] = max( 2, cacheLength );
            _textureCacheVectorMap[deviceName] = vector<ci::gl::Texture2dRef>();
            _textureCacheVectorMap[deviceName].emplace_back( ci::gl::Texture2d::create(1, 1) );
            
        }catch( ... ) {
            cout << "Failed to initialize capture" << std::endl;
        }
        
        return ref;
        
    }
    
    void SiCaptureInput::setDefaultDeviceName( string deviceName ){
        _defaultDeviceName = deviceName;
        cout << "SiCaptureInput::setDefaultDeviceName(\"" << _defaultDeviceName << "\");" << endl << endl;
    }
    
    //! --- サイズの取得
    
    ci::ivec2 SiCaptureInput::getSize(){
        return getSize( _defaultDeviceName );
    }
    ci::ivec2 SiCaptureInput::getSize( string deviceName ){
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
    
    ci::Surface8uRef SiCaptureInput::getSurface(){
        return getSurface( _defaultDeviceName );
    }
    ci::Surface8uRef SiCaptureInput::getSurface( string deviceName ){
        return _captureRefMap[deviceName]->getSurface();
    }
    
    // --- Texture の取得
    
    ci::gl::Texture2dRef SiCaptureInput::getTexture( int cacheAt ){
        return getTexture( _defaultDeviceName, cacheAt );
    }
    ci::gl::Texture2dRef SiCaptureInput::getTexture( string deviceName, int cacheAt ){
        int numCached = getNumCached(deviceName);
        if( numCached < cacheAt + 1 ){
            cacheAt = numCached - 1;
        }
        return _textureCacheVectorMap[deviceName].at( _textureCacheVectorMap[deviceName].size() - (cacheAt+1) );
    }
    
    // --- 指定したサイズで Texture を取得
    
    ci::gl::Texture2dRef SiCaptureInput::getResizedTexture( ivec2 size, int cacheAt ){
        return getResizedTexture( size, _defaultDeviceName, cacheAt );
    }
    ci::gl::Texture2dRef SiCaptureInput::getResizedTexture( ivec2 size, string deviceName, int cacheAt ){
        return hrfm::gl::resizeTexture( getTexture(deviceName, cacheAt), size );
    }
    
    // --- 前フレームとの差分テクスチャを取得
    
    ci::gl::Texture2dRef SiCaptureInput::getDiffTexture(){
        return getDiffTexture( _defaultDeviceName );
    }
    ci::gl::Texture2dRef SiCaptureInput::getDiffTexture( string deviceName ){
        if( _diffFboMap.find(deviceName) == _diffFboMap.end() ){
            ci::ivec2 size = getCaptureRef(deviceName)->getSize();
            ci::gl::Fbo::Format format;
            ci::gl::FboRef fbo = ci::gl::Fbo::create( size.x, size.y, format );
            _diffFboMap[deviceName] = fbo;
        }
        return _diffFboMap[deviceName]->getColorTexture();
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
    
    ci::gl::Texture2dRef SiCaptureInput::_updateTexture( string deviceName ){
        vector<ci::gl::Texture2dRef> * vec = &_textureCacheVectorMap[deviceName];
        if( getCaptureRef(deviceName)->checkNewFrame() ){
            vec->emplace_back( ci::gl::Texture::create( *getSurface( deviceName ) ) );
        }else{
            // Diff 用に取っているが3フレ以上同じだった場合不要な処理
            vec->emplace_back( getTexture(deviceName) );
        }
        if( _textureCacheLengthMap[deviceName] < vec->size() ){
            vec->erase(vec->begin());
        }
        return getTexture(deviceName);
    }
    
    ci::gl::Texture2dRef SiCaptureInput::_updateDiffTexture( string deviceName ){
        
        vector<ci::gl::Texture2dRef> * vec = &_textureCacheVectorMap[deviceName];
    
        if( 1 < vec->size() ){
            
            if( _diffFboMap.find(deviceName) == _diffFboMap.end() ){
                ci::ivec2 size = getCaptureRef(deviceName)->getSize();
                ci::gl::Fbo::Format format;
                ci::gl::FboRef fbo = ci::gl::Fbo::create( size.x, size.y, format );
                _diffFboMap[deviceName] = fbo;
            }
            
            // 前フレームとの差分を取得する.
            {
                
                ci::gl::FboRef fbo = _diffFboMap[deviceName];
                ci::gl::ScopedFramebuffer fboScp( fbo );
                ci::gl::ScopedTextureBind tex0Scp( getTexture(deviceName,1), 0 );
                ci::gl::ScopedTextureBind tex1Scp( getTexture(deviceName), 1 );
                ci::gl::ScopedGlslProg    shaderScp( _diffShader );
                ci::gl::ScopedColor       colorScp( 1.0f, 1.0f, 1.0f );
                
                std::pair<ci::ivec2,ci::ivec2> tmpViewport = ci::gl::getViewport();
                ci::gl::pushMatrices();
                {
                    ci::gl::viewport( ivec2(0), fbo->getSize() );
                    ci::gl::setMatricesWindow( fbo->getSize(), true );
                    ci::gl::clear();
                    _diffShader->uniform("tex0", 0);
                    _diffShader->uniform("tex1", 1);
                    _diffShader->uniform("resolution", vec2(fbo->getSize()) );
                    ci::gl::drawSolidRect( fbo->getBounds() );
                }
                ci::gl::popMatrices();
                ci::gl::viewport( tmpViewport );
                
            }
            
        }
        
        return getDiffTexture();
        
    }
    
}}