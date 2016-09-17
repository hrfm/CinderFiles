#pragma once

#include "cinder/gl/gl.h"
#include "cinder/gl/Fbo.h"
#include "DisplayNode.h"

namespace hrfm{ namespace display{
    
    typedef std::shared_ptr<class Stage> StageRef;
    
    class Stage : public DisplayNode{
    public:
        
        static StageRef create( ci::Camera * camera = NULL ){ return StageRef( new Stage(camera) ); }
        
        Stage( ci::Camera * camera = NULL ):DisplayNode(){
            if( camera != NULL )
            {
                _camera[0] = camera;
            }
            this->addEventListener( hrfm::events::Event::RESIZE, this, &Stage::_onResize );
        };
        ~Stage(){};
        
        void addFbo( ci::gl::FboRef fbo, ci::Camera* camera = NULL );
        
        void setCamera( ci::Camera * camera, int index = 0 );
        void setCameraPersp( ci::CameraPersp * cameraPersp, int index = 0 );
        ci::Camera * getCamera( int index = 0 );
        
        virtual void setAutoClear( bool flag = true );
        
        // 描画対象を Stage に外部からしたい時の処理.
        // 必ず end をセットで呼ぶこと!
        virtual void begin( int index = 0 );
        virtual void end();
        
        virtual DisplayNode * addChild( DisplayNode * child );
        
        virtual void draw(bool offscreen = false, int index = 0 );
        
        virtual void drawOffscreen();
        virtual void drawOffscreen( int index );
        
        ci::gl::TextureRef getTexture( int index = 0 );
        
    protected:
        
        void _begin( int index);
        void _end();
        
        void _updateCamera();
        void _onResize( hrfm::events::Event * event );
        
        bool  _autoClear = true;
        
        int _numFbo;
        int _beginIndex = -1;
        std::map<int,ci::gl::FboRef> _fbo;
        
        std::map<int,ci::Camera*> _camera;
        
        std::pair<ci::ivec2,ci::ivec2> _tmpViewportOnBegin;
        
    };
    
}}