#pragma once

#include "BeatFilterBase.h"
#include "DataLoader.h"

using namespace hrfm::utils;

namespace hrfm{ namespace vj{
    
    class BeatFilterTransTunnel : public hrfm::vj::BeatFilterBase{
        
    public:
        
        BeatFilterTransTunnel( float segmentX = 8.0, float segmentY = 8.0, float move = 0.0 ) : BeatFilterBase("BeatFilterTransTunnel.glsl"){
            _segmentX = segmentX;
            _segmentY = segmentY;
            _move     = move;
        };
        
        virtual void setSegments( float segmentX, float segmentY ){
            _segmentX = segmentX;
            _segmentY = segmentY;
        }
        
        virtual void setMove( float move ){
            _move = move;
        }
        
        virtual void _update( BeatContentBase * content ){
            BeatFilterBase::_update( content );
            if( randFloat() < 0.5 ){
                setMove(0.0f);
            }else if( randFloat() < 0.5 ){
                setMove(-1.0f);
            }else{
                setMove(1.0f);
            }
        }
        
    protected:
        
        virtual void prepare(){
            BeatFilterBase::prepare();
            mShader.uniform("segment_x",_segmentX);
            mShader.uniform("segment_y",_segmentY);
            mShader.uniform("move",_move);
        }
        
    private:
        
        float _segmentX;
        float _segmentY;
        float _move;
        
    };

}}