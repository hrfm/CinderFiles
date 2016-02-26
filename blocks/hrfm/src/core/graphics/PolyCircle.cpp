#include "PolyCircle.h"

#include "cinder/app/App.h"

#include "hrfm.h"

namespace hrfm{ namespace graphics{
    
    void PolyCircle::setDrawRange( float beginDegree, float endDegree, int step, float drawStep ){
        _beginDegree = beginDegree;
        _endDegree   = endDegree;
        _step = step;
        setDrawStep( drawStep );
        _resized = true;
        _update();
        
    }
    
    void PolyCircle::setDrawStep( int step ){
        _drawStep = step;
    }
    void PolyCircle::setDrawStep( float step ){
        if( step < 0.0f ){
            step = 0.0f;
        }else if( 1.0f < step ){
            step = 1.0f;
        }
        _drawStep = round((float)_step * step);
    }
    
    void PolyCircle::setThickness( float thickness ){
        _thickness = thickness;
        _resized = true;
        _update();
    }
    
    /*
    void PolyCircle::setVboMode( bool flag ){
        _isVboMode = flag;
        _resized = true;
        _update();
    }
    //*/
    
    void PolyCircle::_init( float thickness ){
        _thickness = thickness;
    }
    
    void PolyCircle::_update(){
        
        if( isResized() || _vertices.size() == 0 ){
            
            {
                
                vec3  center = vec3( width / 2.0f, height / 2.0f, 0.0f );
                if( drawFromCenter ){
                    center = vec3(0.0);
                }
                float radius = width / 2.0f;
                
                _vertices.empty();
                _vertices.clear();
                for( int i=0; i<=_step; i++ ){
                    float degree0 = _beginDegree + (_endDegree-_beginDegree)/(float)_step * (float)i;
                    float radian0 = degree0 / 180 * 3.14159265358979;
                    _vertices.push_back( center + vec3( radius * cos(radian0), radius * sin(radian0), 0.0f ) );
                    _vertices.push_back( center + vec3( (radius-_thickness) * cos(radian0), (radius-_thickness) * sin(radian0), 0.0f ) );
                }
                
            }
        
            /*
            if( _isVboMode ){
                
                ci::TriMesh m;
                int numberVertices;
                vec3 v0, v1, v2, v3, v01, v02, norm;
                
                for( int i=0; i<_vertices.size()-2; i+=2 ){
                    
                    v0 = _vertices.at(i);
                    v1 = _vertices.at(i+1);
                    v2 = _vertices.at(i+2);
                    v3 = _vertices.at(i+3);
                    
                    {
                        v01 = v1-v0;
                        v02 = v2-v0;
                        norm = ci::cross(v01,v02);
                        
                        m.appendPosition(v0);
                        m.appendNormal(norm);
                        //m.appendTexCoord(vec2(v0.x+0.5,1.0-(v0.y+0.5)));
                        m.appendPosition(v1);
                        m.appendNormal(norm);
                        //m.appendTexCoord(vec2(v1.x+0.5,1.0-(v1.y+0.5)));
                        m.appendPosition(v2);
                        m.appendNormal(norm);
                        //m.appendTexCoord(vec2(v2.x+0.5,1.0-(v2.y+0.5)));
                        numberVertices = m.getNumVertices();
                        m.appendTriangle( numberVertices - 3, numberVertices - 2, numberVertices - 1 );
                    }
                    {
                        v01 = v2-v3;
                        v02 = v1-v3;
                        norm = ci::cross(v01,v02);
                        
                        m.appendPosition(v3);
                        m.appendNormal(norm);
                        //m.appendTexCoord(vec2(v3.x+0.5,1.0-(v3.y+0.5)));
                        m.appendPosition(v2);
                        m.appendNormal(norm);
                        //m.appendTexCoord(vec2(v2.x+0.5,1.0-(v2.y+0.5)));
                        m.appendPosition(v1);
                        m.appendNormal(norm);
                        //m.appendTexCoord(vec2(v1.x+0.5,1.0-(v1.y+0.5)));
                        numberVertices = m.getNumVertices();
                        m.appendTriangle( numberVertices - 3, numberVertices - 2, numberVertices - 1 );
                        
                    }
                    
                }
                
                _vboMesh = ci::gl::VboMesh::create(m);
            
            }else if( _vboMesh != NULL ){
                    
                _vboMesh = NULL;
                
            }
            //*/
            
        }
    }
    
    void PolyCircle::_draw(){
        /*
        if( _isVboMode ){
            ci::gl::draw( _vboMesh );
        }else{
        //*/
            if( 1.0 < _thickness ){
                ci::gl::begin(GL_TRIANGLE_STRIP);
                for( int i=0; i<=_drawStep; i++ ){
                    ci::gl::vertex( _vertices[(i*2)] );
                    ci::gl::vertex( _vertices[(i*2)+1] );
                }
            }else{
                ci::gl::begin(GL_LINE_STRIP);
                for( int i=0; i<=_drawStep; i++ ){
                    ci::gl::vertex( _vertices[(i*2)] );
                }
            }
            ci::gl::end();
        //}
    }
    
}}