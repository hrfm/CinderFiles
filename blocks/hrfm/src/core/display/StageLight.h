#pragma once

#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "Stage.h"

namespace hrfm{ namespace display{
    
    class StageLight{
        
    public:
        
        StageLight(){
            StageLight( ci::ivec2( 1024, 1024 ) );
        }
        StageLight( ci::ivec2 size, ci::vec3 pos = ci::vec3( 0.0f, 0.0f, 1.0f ), ci::vec3 lookAt = ci::vec3(0.0) ){
            this->init( size, pos, lookAt );
        }
        ~StageLight(){}
        
        virtual void init( ci::ivec2 size, ci::vec3 pos, ci::vec3 lookAt );
        
        virtual void setPosition( ci::vec3 pos );
        virtual void setPosition( ci::vec3 pos, ci::vec3 lookAt );
        virtual ci::vec3 getPosition();
        
        virtual void setLookAt( ci::vec3 lookAt );
        virtual ci::vec3 getLookAt();
        
        virtual void renderDepthMap( Stage * stage );
        virtual ci::gl::Texture2dRef getDepthTexture();
        
        virtual ci::vec3 getMVLightPos( ci::mat4 modelViewMatrix );
        virtual ci::mat4 getShadowMatrix();
        
    protected:
        
        ci::CameraPersp      _cam;
        ci::vec3             _position;
        ci::vec3             _lookAt;
        ci::gl::Texture2dRef _shadowMapTex;
        ci::gl::FboRef       _fbo;
        
    };
    
}}