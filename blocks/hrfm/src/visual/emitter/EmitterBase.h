#pragma once

#include "DisplayNode.h"

using namespace ci;
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
            this->baseX = this->position.x;
            this->baseY = this->position.y;
        }
        
        // @param object     emitt する EmittBaseObject
        // @param forceEmitt stage に所属していなくても emitt するか
        void emitt( EmitterBase * object, bool forceEmitt = false ){
            if( forceEmitt ){
                object->baseX = baseX + this->position.x;
                object->baseY = baseY + this->position.y;
                addChild( object );
            }
        }
        
        virtual void draw(){
            if( visible == false || colorA.a <= 0.0f ){
                return;
            }
            _draw();
            _drawChildren();
        }
        
        float baseX, baseY;
        
    protected:
        
    };
    
}}};