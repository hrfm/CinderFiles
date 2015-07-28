#pragma once

#include "BeatFilterBase.h"
#include "DataLoader.h"

using namespace hrfm::utils;

namespace hrfm{ namespace vj{

    class BeatFilterSplit : public hrfm::vj::BeatFilterBase{
        
    public:
        
        BeatFilterSplit() : BeatFilterBase("BeatFilterSplit.glsl"){
            _segments = Vec2f(1.0f,1.0f);
            _rotate = 0;
        };
        
        virtual void update(){
            _segments = Vec2f( (float)randInt( 1, 3 ), (float)randInt( 1, 3 ) );
            _rotate   = randInt(4);
            /*
            _segments = Vec2f(2.0f,2.0f);
            _rotate = 3;
            cout << "segments : " << _segments << endl;
            cout << "rotate   : " << _rotate << endl;
            //*/
        }
        
    protected:
        
        virtual void prepare(){
            BeatFilterBase::prepare();
            mShader.uniform("segments",_segments);
            mShader.uniform("rotate",_rotate);
        }
        
    private:
        
        Vec2f _segments;
        int _type;
        int _rotate;
        
    };

}}