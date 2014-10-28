#pragma once

#include "VisualBase.h"

using namespace sgmnt;
using namespace sgmnt::app;
using namespace sgmnt::utils;

using namespace fl::display;

namespace sgmnt { namespace visual{
    
    class StackedVisualBase : public VisualBase{
        
    public:
        
        StackedVisualBase( AppBase * app, Vec2i resolution, vector<VisualBase*> visuals ){
            VisualBase();
            setup( app, resolution, visuals );
        };
        
        virtual void setup( AppBase * app, Vec2i resolution, vector<VisualBase*> visuals ){
            VisualBase::setup( app, resolution );
            mVisuals = visuals;
        };
        
        virtual void addVisual( VisualBase * visual ){
            mVisuals.push_back( visual );
        }
        
        virtual void update(){
            vector<VisualBase*>::iterator it;
            const std::vector<VisualBase*>::const_iterator & end = mVisuals.end();
            for (it = mVisuals.begin(); it != end; ++it) {
                (*it)->update();
            }
            VisualBase::update();
        };
        
        protected :
        
        void mUpdate(){
            vector<VisualBase*>::iterator it;
            const std::vector<VisualBase*>::const_iterator & end = mVisuals.end();
            gl::enableAdditiveBlending();
            gl::clear();
            gl::color( ColorA( 1.0, 1.0, 1.0, 1.0 ) );
            for (it = mVisuals.begin(); it != end; ++it) {
                gl::draw( (*it)->output(), mFboSize );
            }
            gl::disableAlphaBlending();
        };
        
        vector<VisualBase*> mVisuals;
        
    };
    
}};