#include "Stage.h"

using namespace ci;

namespace hrfm{ namespace display{
    
    void Stage::addFbo( ci::gl::FboRef fbo, ci::Camera* camera ){
        _fbo[_numFbo] = fbo;
        if( camera != NULL ){
            _camera[_numFbo] = camera;
        }
        _numFbo++;
    }

    void Stage::setCamera( ci::Camera * camera, int index ){
        _camera[index] = camera;
    }
    
    void Stage::setCameraPersp( ci::CameraPersp * cameraPersp, int index ){
        setCamera( cameraPersp, index );
    }
    
    ci::Camera * Stage::getCamera( int index ){
        return _camera[index];
    }
    
    void Stage::setAutoClear( bool flag ){
        _autoClear = flag;
        if( _autoClear == false ){
            std::map<int,ci::gl::FboRef>::iterator it, end;
            for( it = _fbo.begin(), end = _fbo.end(); it!=end; it++ ){
                it->second->bindFramebuffer();
                ci::gl::clear();
                it->second->unbindFramebuffer();
            }
        }
    }
    
    DisplayNode * Stage::addChild( DisplayNode * child ){
        eraseFromChildren(child);
        child->_setStage(this);
        child->_setParent(this);
        children.push_back(child);
        return child;
    }
    
    void Stage::draw( bool offscreen, int index ){
        this->begin(index);
        {
            _draw();
            _drawChildren( &colorA );
        }
        this->end();
        if( !offscreen ){
            ci::gl::translate( x, y );
            ci::gl::draw( getTexture() );
        }
    }
    
    void Stage::drawOffscreen(){
        std::map<int,ci::gl::FboRef>::iterator it, end;
        for( it = _fbo.begin(), end = _fbo.end(); it!=end; it++ ){
            draw(true, it->first);
        }
    }
    
    void Stage::drawOffscreen( int index ){
        draw(true, index);
    }
    
    void Stage::begin( int index ){
        _tmpViewportOnBegin = ci::gl::getViewport();
        _begin( index );
    }
    void Stage::end(){
        _end();
        ci::gl::viewport( _tmpViewportOnBegin );
    }
    
    ci::gl::TextureRef Stage::getTexture(int index){
        return this->_fbo[index]->getColorTexture();
    }
    
    ci::gl::FboRef Stage::getFbo( int index ){
        return this->_fbo.at(index);
    }

    void Stage::_begin( int index ){
        
        // 既に何か begin している場合は何もしない.
        if( 0 <= _beginIndex ){
            return;
        }
        
        _fbo[index]->bindFramebuffer();
        
        _beginIndex = index;
        
        ci::gl::enableAlphaBlending();
        ci::gl::color( colorA );
        ci::gl::pushMatrices();
        
        if( _autoClear == true ){
            ci::gl::clear( ColorA(0.0,0.0,0.0,0.0) );
        }
        
        if( _camera.find(index) != _camera.end() ){
            ci::gl::setMatrices( *_camera[index] );
        }else{
            ci::gl::setMatricesWindow( width, height );
        }
        
        ci::gl::viewport( getSize() );
        
    }
    
    void Stage::_end(){
        
        if( _beginIndex < 0 || _fbo.size() <= _beginIndex ){
            return;
        }
        
        ci::gl::popMatrices();
        ci::gl::disableAlphaBlending();
        
        _fbo[_beginIndex]->unbindFramebuffer();
        
        _beginIndex = -1;
        
    }
    
    void Stage::_onResize( hrfm::events::Event * event ){
        
        cout << "_onResize" << endl;
        
        ci::gl::Fbo::Format format;
        
        _fbo.clear();
        _fbo[0] = ci::gl::Fbo::create( width, height, format );
        if( _numFbo < 1 ){
            _numFbo = 1;
        }
        
        // --- Update Camera Aspect Ratio.
        for( std::map<int,ci::Camera*>::iterator it=_camera.begin(); it!=_camera.end(); ++it ){
            it->second->setAspectRatio((float)width / (float)height);
        }
        
    }
    
}}