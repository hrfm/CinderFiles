#pragma once

#include "BeatFilterBase.h"
#include "DataLoader.h"

using namespace hrfm::utils;

namespace hrfm{ namespace vj{
    
    class BeatFilterColor : public hrfm::vj::BeatFilterBase{
    
    public:
        
        BeatFilterColor( float speed = 5.0, float chroma = 1.0 ) : BeatFilterBase("BeatFilterColor.glsl"){
            _speed  = speed;
            _chroma = chroma;
        };
        
        virtual void _update( BeatContentBase * content ){
            BeatFilterBase::_update( content );
        }
        
    protected:
        
        virtual void prepare(){
            BeatFilterBase::prepare();
            
            SiKORGMIDIInterface * korg = &SiKORGMIDIInterface::getInstance();
            
            if( true ){
                mShader.uniform( "speed" , pow( korg->nanoKontrolFader[3], 2.0f ) );
                mShader.uniform( "chroma", korg->nanoKontrolFader[4] );
            }else{
                mShader.uniform( "speed", _speed );
                mShader.uniform( "chroma", _chroma );
            }
        }
        
    private:
        
        float _speed;
        float _chroma;
        
    };

}}