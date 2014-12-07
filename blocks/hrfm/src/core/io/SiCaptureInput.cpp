#include "SiCaptureInput.h"

namespace hrfm { namespace io{
    
    void SiCaptureInput::showAllDevices(){
        // print the devices
        cout << endl << "SiCaptureInput::showAllDevices()" << endl;
        for( auto device = ci::Capture::getDevices().begin(); device != ci::Capture::getDevices().end(); ++device ) {
            cout << "- " << (*device)->getName() << " " << endl;
        }
        cout << endl;
    }
    
    ci::CaptureRef SiCaptureInput::createRef( int width, int height, string deviceName ){
        
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
        }catch( ... ) {
            cout << "Failed to initialize capture" << std::endl;
        }
        
        return ref;
        
    }
    
    ci::Vec2i SiCaptureInput::getSize( string deviceName ){
        return _captureRefMap[deviceName]->getSize();
    }
    
    void SiCaptureInput::update( string deviceName ){
        updateTexture( deviceName );
        updateDiffTexture( deviceName );
    }
    
    ci::gl::Texture SiCaptureInput::updateTexture( string deviceName ){
        if( _beforeFrameMap.find(deviceName) == _beforeFrameMap.end() ){
            _beforeFrameMap[deviceName] = 0;
        }else{
            int currentFrame = getElapsedFrames();
            int beforeFrame  = _beforeFrameMap[deviceName];
            if( beforeFrame < currentFrame ){
                _beforeFrameMap[deviceName] = currentFrame;
                _texMap[deviceName] = ci::gl::Texture( getSurface( deviceName ) );
            }
        }
        return getTexture(deviceName);
    }
    
    ci::gl::Texture SiCaptureInput::updateDiffTexture( string deviceName ){
        
        if( _beforeTextureBeforeFrameMap.find(deviceName) == _beforeTextureBeforeFrameMap.end() ){
            _beforeTextureBeforeFrameMap[deviceName] = 0;
        }
        
        int currentFrame = getElapsedFrames();
        int beforeFrame  = _beforeTextureBeforeFrameMap[deviceName];
        
        if( beforeFrame != currentFrame ){
            
            if( _beforeTextureMap.find(deviceName) != _beforeTextureMap.end() ){
                
                Area viewport = ci::gl::getViewport();
                
                if( _diffFboMap.find(deviceName) == _diffFboMap.end() ){
                    ci::Vec2i size = getCaptureRef(deviceName)->getSize();
                    ci::gl::Fbo::Format format;
                    ci::gl::Fbo fbo = ci::gl::Fbo( size.x, size.y, format );
                    _diffFboMap[deviceName] = fbo;
                }
                
                ci::gl::Texture tex = getTexture(deviceName);
                ci::gl::Fbo     fbo = _diffFboMap[deviceName];
                
                // 前フレームとの差分を取得する.
                fbo.bindFramebuffer();
                {
                    ci::gl::pushMatrices();
                    ci::gl::setViewport( fbo.getBounds() );
                    ci::gl::setMatricesWindow( fbo.getSize(), false );
                    {
                        ci::gl::clear();
                        ci::gl::color(1.0f,1.0f,1.0f);
                        _beforeTextureMap[deviceName].bind(0);
                        tex.bind(1);
                        _diffShader.bind();
                        {
                            _diffShader.uniform("tex0", 0);
                            _diffShader.uniform("tex1", 1);
                            _diffShader.uniform("resolution", Vec2f(fbo.getSize()) );
                            ci::gl::drawSolidRect( fbo.getBounds() );
                        }
                        _diffShader.unbind();
                        tex.unbind();
                        _beforeTextureMap[deviceName].unbind();
                    }
                    ci::gl::popMatrices();
                }
                fbo.unbindFramebuffer();
                
                ci::gl::setViewport(viewport);
                
            }
            
            _beforeTextureMap[deviceName]            = getTexture(deviceName);
            _beforeTextureBeforeFrameMap[deviceName] = currentFrame;
            
        }
        
        return getDiffTexture();
        
    }
    
    ci::CaptureRef SiCaptureInput::getCaptureRef( string deviceName ){
        return _captureRefMap[deviceName];
    }
    
    ci::Surface SiCaptureInput::getSurface( string deviceName ){
        return _captureRefMap[deviceName]->getSurface();
    }
    
    ci::gl::Texture SiCaptureInput::getTexture( string deviceName ){
        if( _texMap.find(deviceName) == _texMap.end() ){
            _texMap[deviceName] = ci::gl::Texture(1,1);
        }
        return _texMap[deviceName];
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
    
}}