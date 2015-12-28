#include "NumberText.h"

namespace hrfm{ namespace graphics{
    
    void NumberText::setNumber( double number ){
        setNumber( number, _speed );
    }
    
    void NumberText::setNumber( double number, double speed ){
        
        if( 0.0 < _maxValue ){
            _number = min(number,_maxValue);
        }else{
            _number = number;
        }
        
        _speed  = speed;
        
    }
    
    void NumberText::_update(){
        if( 1.0 <= _speed ){
            _drawNumber = _number;
        }else{
            if( _drawNumber != _number ){
                _drawNumber += ( _number - _drawNumber ) * _speed;
                if( _number - _drawNumber < 0.001 ){
                    _drawNumber = _number;
                }
            }
        }
        
        if( _numDigits <= 0 && _numDecimalDigits <= 0 ){
            
            _drawString = toString( floor(_drawNumber) );
            
        }else{
            
            double decimal = pow(10.0,_numDecimalDigits);
            double x = round(_drawNumber*decimal) / decimal;
            double a,b;
            
            b = modf(x,&a);
            
            string aStr = "", bStr = "";
            if( 0 < _numDigits ){
                aStr = toString( a );
                while( aStr.length() < _numDigits ){
                    aStr = "0" + aStr;
                }
            }
            if( 0 < _numDecimalDigits ){
                bStr = toString( round(b*decimal) );
                while( bStr.length() < _numDecimalDigits ){
                    bStr = bStr + "0";
                }
                bStr = "."+bStr;
            }
            
            _drawString = aStr+bStr;
            
        }
        
    }
    
    void NumberText::_draw(){
        _fontRef->drawString( _drawString, vec2(0.0) );
    }
    
}}