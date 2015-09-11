#pragma once

#include "VboNode.h"

namespace hrfm{ namespace gl{
    
    class VboWall : public VboNode{
    
    public:
        
        VboWall():VboNode(){};
        ~VboWall(){};
        
        virtual void setup();
        virtual void clear();
        
        void setTexture( ci::gl::Texture * tex );
        
    protected:
        
        virtual void _update( ci::CameraPersp * camera );
        virtual void _draw( ci::CameraPersp * camera );
        
    private:
        
        ci::gl::Texture * _texture = NULL;
        
    };
    
}}