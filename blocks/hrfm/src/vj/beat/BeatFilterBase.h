#pragma once

#include "hrfm.h"
#include "FilterBase.h"
#include "SiBPM.h"

namespace hrfm{ namespace vj{

    class BeatFilterBase : public hrfm::gl::FilterBase{
        
    public:
        
        BeatFilterBase() : FilterBase(){}
        BeatFilterBase( string fragmentShader ) : FilterBase( fragmentShader ){}
        BeatFilterBase( string fragmentShader, string vertexShader ) : FilterBase( fragmentShader, vertexShader ){}
        
        void update( BeatContentBase * content ){
            if( updated == true ){
                return;
            }
            _update( content );
            updated = true;
        }
        
        int type = 2;
        bool updated;
        
    protected:
        
        virtual void _update( BeatContentBase * content ){
            type = randInt(3);
            _content = content;
        }
        
        virtual void prepare(){
            int position = SiBPM::getInstance().position * 1000000;
            mShader.uniform( "bpm_position" , position / 1000000.0f );
            mShader.uniform( "type", type );
            if( _content != NULL ){
                mShader.uniform( "has_direction", _content->hasDirection?1:0 );
            }else{
                mShader.uniform( "has_direction", 0 );
            }
        }
        
        BeatContentBase * _content = NULL;
        
    };
    
}}