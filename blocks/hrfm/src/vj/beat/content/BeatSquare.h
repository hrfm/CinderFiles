#pragma once

#include "BeatContentBase.h"

namespace hrfm{ namespace vj{
    
    class BeatSquare : public hrfm::vj::BeatContentBase{
    public:
        BeatSquare(){
            hrfm::vj::BeatContentBase();
        }
        virtual void draw( double position, int width, int height );
    };
    
    class BeatLine : public hrfm::vj::BeatContentBase{
    public:
        BeatLine(){
            hrfm::vj::BeatContentBase();
            hasDirection = true;
        }
        virtual void draw( double position, int width, int height );
    };
    
    class BeatCircle : public hrfm::vj::BeatContentBase{
    public:
        BeatCircle(){
            hrfm::vj::BeatContentBase();
        }
        virtual void draw( double position, int width, int height );
    };
    
    // --- Audio
    
    class BeatHoge : public hrfm::vj::BeatContentBase{
    public:
        BeatHoge( bool random = false ){
            
            hrfm::vj::BeatContentBase();
            
            hasDirection = false;
            
            _type   = 0;
            _random = random;
            
            float halfW = 0.5;
            float halfH = 0.5;
            float w = 0.5 * 0.7;
            float h = 0.5 * 0.7;
            float ox = ( 0.5 - w ) / 2.0;
            float oy = ( 0.5 - h ) / 2.0;
            
            _rect0 = Rectf( ox, oy, ox + w, oy + h );
            _rect1 = Rectf( ox + halfW, oy, ox + halfW + w, oy + h );
            _rect2 = Rectf( ox, oy + halfH, ox + w, oy + halfH + h );
            _rect3 = Rectf( ox + halfW, oy + halfH, ox + halfW + w, oy + halfH + h );
            
        }
        virtual void setup();
        virtual void draw( double position, int width, int height );
    private:
        int  _type;
        bool _random;
        Rectf _rect0;
        Rectf _rect1;
        Rectf _rect2;
        Rectf _rect3;
        vector<Rectf> _order;
    };
    
    // --- Audio
    
    class BeatEqualizer : public hrfm::vj::BeatContentBase{
    public:
        BeatEqualizer(){
            hrfm::vj::BeatContentBase();
            hasDirection = true;
        }
        virtual void draw( double position, int width, int height );
    };
    
    class BeatAudioWave : public hrfm::vj::BeatContentBase{
    public:
        BeatAudioWave(){
            hrfm::vj::BeatContentBase();
        }
        virtual void draw( double position, int width, int height );
    };
    
}}