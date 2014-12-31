#pragma once

#include "hrfm.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace hrfm::events;

namespace hrfm{ namespace visual{ namespace emitter{
    
    class EmitterBase : public hrfm::display::DisplayNode{
        
    public:
        
        EmitterBase(){
            hrfm::display::DisplayNode();
            init();
        }
        
        virtual void init(){
            this->baseX = x;
            this->baseY = y;
        }
        
        // @param object     emitt する EmittBaseObject
        // @param forceEmitt stage に所属していなくても emitt するか
        void emitt( EmitterBase * object, bool forceEmitt = false ){
            if( forceEmitt || hasStage() ){
                object->baseX = baseX + x;
                object->baseY = baseY + y;
                addChild( object );
            }
        }
        
        virtual void draw(){
            if( visible == false || alpha <= 0.0f ){
                return;
            }
            _draw();
            _drawChildren();
        }
        
        float baseX, baseY;
        
    protected:
        
    };
    
}}};