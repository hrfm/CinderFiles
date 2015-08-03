#pragma once

#include "BeatFilterBase.h"
#include "DataLoader.h"

using namespace hrfm::utils;

namespace hrfm{ namespace vj{
    
    class BeatFilterDot : public hrfm::vj::BeatFilterBase{
        
    public:
        
        BeatFilterDot() : BeatFilterBase("BeatFilterDot.glsl"){};
        
        virtual void _update( BeatContentBase * content ){
            BeatFilterBase::_update( content );
            _mtx = Vec2f( 1920.0f, 1080.0f );
        }
        
    protected:
        
        virtual void prepare(){
            
            BeatFilterBase::prepare();
            
            float p = SiBPM::getInstance().position;
            if( p < 0.4 ){
                mShader.uniform("matrix", _mtx );
            }else{
                p = ( p - 0.4 ) /0.6;
                p = p*p;
                mShader.uniform("matrix", _mtx / (100.0f*p) );
            }
            mShader.uniform("segments",0.1f);
            
        }
        
    private:
        
        Vec2f _mtx;
        
    };

}}