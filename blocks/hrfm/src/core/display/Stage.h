#pragma once

#include "cinder/gl/gl.h"
#include "cinder/gl/Fbo.h"
#include "DisplayNode.h"
#include "StageLight.h"

namespace hrfm{ namespace display{
    
    class StageLight;
    
    class Stage : public DisplayNode{
    public:
        
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
        
        virtual int  numLights();
        virtual void addLight( StageLight * light );
        virtual void removeLight( StageLight * light );
        
        virtual void draw(bool offscreen = false);
        virtual void drawOffscreen();
        
        ci::gl::TextureRef getTexture();
    
    protected:
        
        friend class StageLight;
        
    private:
        
        void _begin();
        void _end();
        
        void _beginLights();
        void _endLights();
        
        void _updateCamera();
        void _onResize( hrfm::events::Event * event );
        
        bool  _autoClear = true;
        ci::CameraPersp * _cameraPersp;
        ci::gl::FboRef _fbo;
        std::pair<ci::ivec2,ci::ivec2> _tmpViewportOnBegin;
        
        bool eraseLightFromLights( StageLight * light );
        vector<StageLight*> _lights;
        
    };
    
}}