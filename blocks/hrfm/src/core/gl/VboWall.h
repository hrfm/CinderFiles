#pragma once

#include "VboNode.h"
#include "SiAudioInput.h"

namespace hrfm{ namespace gl{
    
    class VboWall : public VboNode{
    
    public:
        
        VboWall():VboNode(){};
        ~VboWall(){};
        
        virtual void setup( Vec2i segments, bool curve = false, float noise = false );
        virtual void clear();
        
        void setTexture( ci::gl::Texture * tex );
        
    protected:
        
        virtual void _update( ci::CameraPersp * camera );
        virtual void _draw( ci::CameraPersp * camera );
        ci::gl::Texture * _texture = NULL;
        
    };
    
}}