#pragma once

#include "hrfm.h"
#include "cinder/gl/gl.h"

namespace hrfm{ namespace display{
    
    class TessellationPlane : public hrfm::display::DisplayNode{
        
    public:
        
        TessellationPlane( float num = 4.0, ci::gl::TextureRef tex = nullptr ):hrfm::display::DisplayNode(){
            this->init( num, tex );
        };
        
        virtual void init( float num, ci::gl::TextureRef tex = nullptr );
        
        virtual void setEnableWireframe( bool flag );
        
    protected:
        
        hrfm::display::VertBatchNode * _batchNode;
        
        virtual void _update();
        virtual void _draw();
        
    };
    
}}