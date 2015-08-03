#pragma once

#include "BeatFilterBase.h"
#include "DataLoader.h"

using namespace hrfm::utils;

namespace hrfm{ namespace vj{
    
    class BeatFilterTransField : public hrfm::vj::BeatFilterBase{
        
    public:
        
        BeatFilterTransField( bool vertical = false, float move = 0.0 ) : BeatFilterBase("BeatFilterTransField.glsl"){
            _vertical = vertical ? 1 : 0;
            _move     = move;
        };
        
    protected:
        
        virtual void prepare(){
            BeatFilterBase::prepare();
            mShader.uniform("vertical",_vertical);
            //if( _content && _content->hasDirection ){
                //mShader.uniform("move",0.0f);
            //}else{
                mShader.uniform("move",_move);
            //}
        }
        
    private:
        
        int   _vertical;
        float _move;
        
    };

}}