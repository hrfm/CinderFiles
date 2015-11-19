#pragma once

#include "VboNode.h"
#include "SiAudioInput.h"

namespace hrfm{ namespace gl{
    
    class VboWall : public hrfm::display::VboNode{
    
    public:
        
        VboWall():hrfm::display::VboNode(){};
        ~VboWall(){};
        
        virtual void setup( ivec2 segments, bool curve = false, float noise = false );
        virtual void clear();
        
        void setTexture( ci::gl::TextureRef tex );
        
    protected:
        
        virtual void _update( ci::CameraPersp * camera );
        virtual void _draw( ci::CameraPersp * camera );
        ci::gl::TextureRef _texture = NULL;
        
    };
    
}}