#pragma once

#include "VboNode.h"
#include "DisplayUtil.h"

#include "cinder/Camera.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/Fbo.h"
//#include "cinder/gl/Light.h"

#include "Utils.h"

namespace hrfm{ namespace gl{
    
    class VboStage : public VboNode{
        
    public:
        
        VboStage( ci::CameraPersp * camera = NULL ):VboNode(){
            this->width  = 0.0;
            this->height = 0.0;
            this->_fov   = 30.0;
            if( camera == NULL ){
                this->camera = new ci::CameraPersp();
            }else{
                this->camera = camera;
            }
            this->addEventListener( hrfm::events::Event::RESIZE, this, &VboStage::_onResize );
        };
        ~VboStage(){};
        
        float getFov();
        void setFov( float fov );
        
        ci::ivec2 getSize();
        void setSize( int w, int h );
        void setSize( ci::ivec2 size );
        
        virtual VboNode * addChild( VboNode * child );
        
        //!!!!! virtual void addLight( ci::gl::Light * light );
        //!!!!! virtual void removeLight( ci::gl::Light * light );
        
        virtual void update();
        virtual void draw();
        virtual void draw( ci::Rectf bounds );
        
        ci::gl::Texture2dRef getTexture();
        
        float width, height;
        ci::CameraPersp * camera;
        
    protected:
        
    private:
        
        ci::gl::FboRef _fbo;
        void _onResize( hrfm::events::Event * event );
        //!!!!! bool eraseLightFromLights( ci::gl::Light * light );
        
        float _fov;
        int _beforeWidth;
        int _beforeHeight;
        
        //!!!!! vector<ci::gl::Light*> _lights;
        
    };
    
}}