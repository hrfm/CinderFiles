#include "RandomText.h"

namespace hrfm{ namespace graphics{
    
    string RandomText::_CHAR_LIST = "1234567890qwertyuiop[]\\';lkjhgfdsazxcvbnm,./@#$%^&*()_+~<>?:\"{}| ";
    
    void RandomText::setText( string text ){
        _text = text;
        visible = 0 < _text.length();
    }
    
    void RandomText::setText( string text, double delay, double interval ){
        setText( text );
        randomize( delay, interval );
    }
    
    void RandomText::randomize( double delay, double interval ){
        _delay    = delay;
        _interval = interval;
        _startSec = ci::app::getElapsedSeconds();
    }
    
    void RandomText::_init(){
        _delay = 0.0;
    }
    
    void RandomText::_update(){
        
        double now = ci::app::getElapsedSeconds();
        
        std::random_device rnd;
        std::mt19937 mt(rnd());
        std::uniform_real_distribution<float> rndm(0.0,1.0);
        
        if( _delay == 0.0 || (now-_startSec) < _delay ){
            _drawText = "";
            for( int i=0; i<_text.length(); i++ ){
                _drawText += _CHAR_LIST[floor(rndm(mt)*_CHAR_LIST.size())];;
            }
        }else if( _interval <= 0.0 ){
            _drawText = _text;
        }else{
            int numFixed = floor( (now-_startSec-_delay)/_interval );
            if( _text.length() <= numFixed ){
                _drawText = _text;
            }else{
                _drawText = "";
                for( int i=0; i<_text.length(); i++ ){
                    if( i < numFixed ){
                        _drawText += _text[i];
                    }else{
                        _drawText += _CHAR_LIST[floor(rndm(mt)*_CHAR_LIST.size())];;
                    }
                }
            }
        }
        
    }
    
    void RandomText::_draw(){
        SiFontUtil::getInstance().getTextureFontRef(_fontName,_fontSize)->drawString( _drawText, vec2(0.0) );
    }
    
}}