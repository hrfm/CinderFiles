#pragma once

#include "cinder/app/App.h"
#include "hrfm.h"

namespace hrfm{ namespace graphics{
    
    class NumberText:public hrfm::display::DisplayNode{
        
    public:
        
        NumberText( double number = 0.0, int numDigits = 2, int numDecimalDigits = 0, float fontSize=8.0f, string fontName="Monaco" ):hrfm::display::DisplayNode(){
            
            _drawNumber = number;
            _numDigits  = numDigits;
            _numDecimalDigits = numDecimalDigits;
            _maxValue   = ( 0 < _numDigits ) ? pow(10.0,_numDigits) - 1.0 : 0.0;
            
            _fontSize = fontSize;
            _fontName = fontName;
            _fontRef  = SiFontUtil::getInstance().getTextureFontRef(_fontName,_fontSize);
            
            setNumber(number,1.0);
            
            // --- 描画サイズを計算する.
            
            int len = 0;
            if( 0 < _numDigits ){
                len += _numDigits;
            }
            if( 0 < _numDecimalDigits ){
                len += _numDecimalDigits + 1;
            }
            string sample = "";
            while( sample.length() <= len ){
                sample += "0";
            }
            vector<pair<uint16_t,vec2>> placements = _fontRef->getGlyphPlacements(sample);
            
            this->width( placements.at( placements.size()-1 ).second.x );
            
        }
        
        virtual void setNumber( double number );
        virtual void setNumber( double number, double speed );
        
    protected:
        
        int _numDigits; // 整数の桁数
        int _numDecimalDigits; // 小数点の桁数;
        
        ci::gl::TextureFontRef _fontRef;
        float  _fontSize;
        string _fontName;
        
        double _number;
        double _drawNumber;
        string _drawString;
        double _speed;
        double _maxValue;
        
        virtual void _update();
        virtual void _draw();
        
    };
    
}}