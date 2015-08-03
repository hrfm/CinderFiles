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
        
        virtual void setRandom( bool flag ){
            isRandom = flag ? 1 : 0;
        }
        
        int type = 0;
        int isRandom = true;
        bool updated;
        
    protected:
        
        virtual void _update( BeatContentBase * content ){
            if( isRandom == true ){
                type = randInt(3);
            }
            _content = content;
        }
        
        virtual void prepare(){
            mShader.uniform( "bpm_position" , (float)SiBPM::getInstance().position );
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