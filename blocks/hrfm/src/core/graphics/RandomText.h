#pragma once

#include "cinder/app/App.h"
#include "hrfm.h"

namespace hrfm{ namespace graphics{
    
    class RandomText:public hrfm::display::DisplayNode{
        
        static string _CHAR_LIST;
        
    public:
        
        RandomText( string text = "", float fontSize=8.0f, string fontName="Monaco" ):hrfm::display::DisplayNode(){
            _init();
            _fontSize = fontSize;
            _fontName = fontName;
            setText(text);
        }
        
        virtual void setText( string text );
        virtual void setText( string text, double delay, double interval = 0.02 );
        virtual void randomize( double delay = 0.0, double interval = 0.02 );
        
    protected:
        
        string _text;
        string _drawText;
        float  _fontSize;
        string _fontName;
        
        double _delay;
        double _interval;
        double _startSec;
        
        void _init();
        virtual void _update();
        virtual void _draw();
        
    };
    
}}