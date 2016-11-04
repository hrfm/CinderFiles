#pragma once

#include "cinder/gl/gl.h"
#include "cinder/gl/Fbo.h"
#include "DisplayNode.h"

namespace hrfm{ namespace display{
    
    class IStage : public DisplayNode{
    public:
        
        IStage():DisplayNode(){}
        
        virtual DisplayNode * addChild( DisplayNode * child ){
            eraseFromChildren(child);
            child->_setStage(this);
            child->_setParent(this);
            children.push_back(child);
            return child;
        }
    };
    
}}