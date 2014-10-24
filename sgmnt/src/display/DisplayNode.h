#pragma once

#include "IDrawable.h"
#include <list>

namespace sgmnt{ namespace display{
    
    class DisplayNode : public IDrawable{
    
    public :
        
        DisplayNode(){ IDrawable(); };
        ~DisplayNode(){};
        
        virtual int numChildren();
        
        virtual IDrawable * addChild( IDrawable * child );
        virtual IDrawable * removeChild( IDrawable * child );
        
        /*
        virtual IDrawable * addChildAt( IDrawable * child, int index );
        virtual IDrawable * removeChildAt( int index );
        //*/
        
        virtual void update();
        virtual void draw();
        
        std::list<IDrawable*> children;
        
    };
    
}}