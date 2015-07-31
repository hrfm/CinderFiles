#pragma once

#include "BeatFilterBase.h"
#include "DataLoader.h"

using namespace hrfm::utils;

namespace hrfm{ namespace vj{

    typedef struct _split_setting{
        Vec2f segments;
        int   rotate;
    } SplitSetting;
    
    class BeatFilterSplit : public hrfm::vj::BeatFilterBase{
        
    public:
        
        BeatFilterSplit() : BeatFilterBase("BeatFilterSplit.glsl"){
            _segments = Vec2f(1.0f,1.0f);
            _rotate = 0;
        };
        
        virtual void _update( BeatContentBase * content ){
            BeatFilterBase::_update( content );
            if( _sequence != NULL ){
                _segments = _sequence->at(_sequencePointer)->segments;
                _rotate   = _sequence->at(_sequencePointer)->rotate;
                if( _sequence->size() <= ++_sequencePointer ){
                    _sequencePointer = 0;
                }
            }else{
                _segments = Vec2f( (float)randInt( 1, 3 ), (float)randInt( 1, 3 ) );
                _rotate   = randInt(4);
            }
            /*
            cout << "segments : " << _segments << endl;
            cout << "rotate   : " << _rotate << endl;
            //*/
        }
        
        virtual void setSequence( vector<SplitSetting*> * seq ){
            _sequencePointer = 0;
            _sequence = seq;
        }
        
    protected:
        
        virtual void prepare(){
            BeatFilterBase::prepare();
            mShader.uniform("segments",_segments);
            mShader.uniform("rotate",_rotate);
        }
        
    private:
        
        Vec2f _segments;
        int _rotate;
        
        vector<SplitSetting*> * _sequence = NULL;
        int _sequencePointer;
        
    };

}}