#pragma once

#include "cinder/gl/gl.h"
#include "cinder/gl/Fbo.h"
#include "DisplayNode.h"

namespace hrfm{ namespace display{
    
    typedef std::shared_ptr<class Stage> StageRef;
    
    class Stage : public DisplayNode{
    public:
        
        static StageRef create( ci::CameraPersp * camera = NULL ){ return StageRef( new Stage(camera) ); }
        
        Stage( ci::CameraPersp * camera = NULL ):DisplayNode(){
            this->_cameraPersp = camera;
            this->addEventListener( hrfm::events::Event::RESIZE, this, &Stage::_onResize );
        };
        ~Stage(){};
        
        void setCameraPersp( ci::CameraPersp * cameraPersp );
        ci::CameraPersp * getCameraPersp();
        
        virtual void setAutoClear( bool flag = true );
        
        // 描画対象を Stage に外部からしたい時の処理.
        // 必ず end をセットで呼ぶこと!
        virtual void begin();
        virtual void end();
        
        virtual DisplayNode * addChild( DisplayNode * child );
        
        virtual void draw(bool offscreen = false);
        virtual void drawOffscreen();
        
        ci::gl::TextureRef getTexture();
        
    protected:
        
        void _begin();
        void _end();
        
        void _updateCamera();
        void _onResize( hrfm::events::Event * event );
        
        bool  _autoClear = true;
        ci::CameraPersp * _cameraPersp;
        ci::gl::FboRef _fbo;
        std::pair<ci::ivec2,ci::ivec2> _tmpViewportOnBegin;
        
    };
    
}}