#include "PolyLine.h"

namespace hrfm{ namespace graphics{
    
    void PolyLine::addPoint( vec3 point )
    {
        points.push_back(point);
        int size = points.size();
        if( 2 <= size ){
            
            vec3 p0 = points.at( size-2 );
            vec3 p1 = points.at( size-1 );
            
            float dist = ci::distance(p1,p0);
            _totalLength += dist;
            _lengthList.push_back(_totalLength);
            
        }
    }
    
    void PolyLine::clear(){
        points.clear();
        _totalLength = 0;
    }
    
    void PolyLine::setThickness( float thickness ){
        this->thickness = thickness;
    }
    
    void PolyLine::setDrawRange(float from, float to){
        if( from < 0 ){
            from = 0;
        }else if( 1.0 < from ){
            from = 1.0;
        }
        if( to < from ){
            to = from;
        }else if( 1.0 < to ){
            to = 1.0;
        }
        _drawFrom = from;
        _drawTo   = to;
        cout << _drawFrom << "-" << _drawTo << endl;
    }
    
    void PolyLine::setPointScale( vec3 scale ){
        _pointScale = scale;
        if( _pointScale.x != 1.0 || _pointScale.y != 1.0 || _pointScale.z != 1.0 )
        {
            _usePointScale = true;
        }else{
            _usePointScale = false;
        }
    }
    
    void PolyLine::setPointScale( vec3 scale, vec3 pivot ){
        _pointScalePivot = pivot;
        setPointScale(scale);
    }
    
    void PolyLine::setPointScalePivot( vec3 pivot ){
        _pointScalePivot = pivot;
    }
    
    void PolyLine::_update(){
        
        _vertices.empty();
        _vertices.clear();
        
        if( points.size() <= 1 || _drawFrom == _drawTo || _totalLength == 0 ){
            return;
        }
        
        // --------------------------------------
        // ポイントの整理.
        
        vector<vec3> pList;
        
        if( _drawFrom == 0.0 ){
            auto po  = points.at(0);
            if( _usePointScale ){
                po = _pointScalePivot + (po-_pointScalePivot)*_pointScale;
            }
            pList.push_back( po );
        }
        
        if( _drawTo < _lengthList.at(1) / _totalLength ){
            
            auto po  = points.at(0);
            auto p1  = points.at(1);
            
            if( _usePointScale ){
                po = _pointScalePivot + (po-_pointScalePivot)*_pointScale;
                p1 = _pointScalePivot + (p1-_pointScalePivot)*_pointScale;
            }
            
            auto len = _lengthList.at(1) / _totalLength;
            
            pList.push_back( po + (p1-po)*(_drawTo/len) );
            
        }else{
            
            for( int i=1; i<points.size()-1; i++ ){
                
                auto p0 = points.at(i-1);
                auto po = points.at(i);
                auto p1 = points.at(i+1);
                
                if( abs( atan2( po.y-p0.y, po.x-p0.x ) - atan2( p1.y-p0.y, p1.x-p0.x ) ) < 0.0001 || p0 == po ){
                    continue;
                }
                
                if( _usePointScale ){
                    p0 = _pointScalePivot + (p0-_pointScalePivot)*_pointScale;
                    po = _pointScalePivot + (po-_pointScalePivot)*_pointScale;
                    p1 = _pointScalePivot + (p1-_pointScalePivot)*_pointScale;
                }
                
                float len0 = _lengthList.at(i-1) / _totalLength;
                float leno = _lengthList.at(i)   / _totalLength;
                float len1 = _lengthList.at(i+1) / _totalLength;
                
                if( _drawFrom < leno && leno < _drawTo ){
                    if( len0 < _drawFrom ){
                        pList.push_back( p0 + (po-p0)*((_drawFrom-len0)/(leno-len0)) );
                    }
                    pList.push_back(po);
                    if( _drawTo < len1 ){
                        pList.push_back( po + (p1-po)*((_drawTo-leno)/(len1-leno)) );
                        break;
                    }
                }
                
            }
            
            if( _drawTo == 1.0 ){
                auto po  = points.at(points.size()-1);
                if( _usePointScale ){
                    po = _pointScalePivot + (po-_pointScalePivot)*_pointScale;
                }
                pList.push_back( po );
            }
            
        }
        
        if( pList.size() < 2 ){
            return;
        }
        
        // --------------------------------------
        
        double t   = this->thickness / 2.0;
        int    len = pList.size();
        
        vec3   p0, p1;
        double PI2 = 3.141592/2.0;
        double rad;
        
        // points[head] ----
        
        p0  = pList.at(0);
        p1  = pList.at(1);
        rad = atan2( p1.y-p0.y, p1.x-p0.x );
        
        _vertices.push_back( vec3( p0.x + t*cos(rad+PI2), p0.y + t*sin(rad+PI2), 0.0 ) );
        _vertices.push_back( vec3( p0.x + t*cos(rad-PI2), p0.y + t*sin(rad-PI2), 0.0 ) );
        
        // points[] ----
        
        if( 3 <= len ){
            
            double r = t, cosTheta, theta;
            vec2 v0, v1;
            vec3 p0, po, p1;
            
            for( int i=1; i<len-1; i++ ){
                
                p0 = pList.at(i-1);
                po = pList.at(i);
                p1 = pList.at(i+1);
                v0 = vec2( p0.x-po.x, p0.y-po.y );
                v1 = vec2( p1.x-po.x, p1.y-po.y );
                
                // どちらを先にプロットするか、向きで調べる
                auto rot = (p1.x-po.x)*(p0.y-po.y)-(p0.x-po.x)*(p1.y-po.y);
                
                if( rot == 0.0 ){
                    
                    // ここコメントアウトすると動きがやばい.
                    rad = atan2( po.y-p0.y, po.x-p0.x );
                    _vertices.push_back( vec3( po.x + t*cos(rad+PI2), po.y + t*sin(rad+PI2), 0.0 ) );
                    _vertices.push_back( vec3( po.x + t*cos(rad-PI2), po.y + t*sin(rad-PI2), 0.0 ) );
                    
                }else{
                    
                    // マイター処理の長さ.
                    cosTheta = dot(v0,v1) / (length(v0)*length(v1));
                    theta    = acos(cosTheta);
                    r        = thickness / sin(theta/2.0) / 2.0;
                    if( isnan(r) ){ r = t; }
                    
                    // 角の２等分ベクトル.
                    vec2 v = ( length(v1)*v0+length(v0)*v1 ) / ( length(v0) + length(v1) );
                    rad = atan2(v.y,v.x);
                    
                    if( rot < 0 ){
                        _vertices.push_back( vec3( po.x + r*cos(rad+PI2*2), po.y + r*sin(rad+PI2*2), 0.0 ) );
                        _vertices.push_back( vec3( po.x + r*cos(rad), po.y + r*sin(rad), 0.0 ) );
                    }else{
                        _vertices.push_back( vec3( po.x + r*cos(rad), po.y + r*sin(rad), 0.0 ) );
                        _vertices.push_back( vec3( po.x + r*cos(rad+PI2*2), po.y + r*sin(rad+PI2*2), 0.0 ) );
                    }
                    
                }
                
                
            }
            
        }
        
        // points[tail] ----
        
        p0  = pList.at(len-2);
        p1  = pList.at(len-1);
        rad = atan2( p1.y-p0.y, p1.x-p0.x );
        
        _vertices.push_back( vec3( p1.x + t*cos(rad+PI2), p1.y + t*sin(rad+PI2), 0.0 ) );
        _vertices.push_back( vec3( p1.x + t*cos(rad-PI2), p1.y + t*sin(rad-PI2), 0.0 ) );
        
    }
    
    void PolyLine::_draw(){
        
        int i   = 0;
        int len = _vertices.size();
        
        if( len == 0 ){ return; }
        
        if( 1.0 < this->thickness ){
            ci::gl::begin(GL_TRIANGLE_STRIP);
            //ci::gl::begin(GL_LINE_STRIP);
            while( i < len ){
                ci::gl::vertex( _vertices[i++] );
            }
        }else{
            ci::gl::begin(GL_LINE_STRIP);
            while( i < len ){
                ci::gl::vertex( _vertices[i] );
                i+=2;
            }
        }
        ci::gl::end();
        
    }
    
}}